/**
  src/omx_reference_resource_manager.c

  This simple resource manager emulates the behavior of a real RM.
	It applies the rules defined in the OpenMAX spec. It can be replaced in
	the future by a real system.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

  $Date$
  Revision $Rev$
  Author $Author$
*/

#include <string.h>
#include "omx_reference_resource_manager.h"
#include "base/omx_base_component.h"


/** This is the static base pointer of the list
 */
static ComponentListType *componentList = NULL;
static int globalTimestamp;


/** Components specific section
 * Each component that has some resources to be handled by this
 * resource manager should add some fields here
 */
/* Max allowable volume component instance */
#define MAX_RUNNING_COMPONENT_VOLUME 3
/** Maximum Number of Volume Component Instance*/
static OMX_U32 noVolumeResourceAllocated = 0;


/**
 * This debug function is capable of printing the full list
 * actually stored
 */
void RM_printList(int viewFlag) {
	ComponentListType *componentTemp = componentList;
	omx_base_component_PrivateType* omx_base_component_Private;
	int index;

	if (!componentList) {
		printf("The list is empty\n");
		return;
	}
	index = 0;
	while (componentTemp) {
		omx_base_component_Private = (omx_base_component_PrivateType*)componentTemp->openmaxStandComp->pComponentPrivate;
		if ((viewFlag & RM_SHOW_NAME) == RM_SHOW_NAME) {
			printf("Name %s ", omx_base_component_Private->name);
		}
		if ((viewFlag & RM_SHOW_ADDRESS) == RM_SHOW_ADDRESS) {
			printf("Address %x ", (int)componentTemp->openmaxStandComp);
		}
		printf("\n");
		index++;
		componentTemp = componentTemp->next;
	}
}

/** This function returns the number of components that have a lower priority
 * than the value specified, and the lowest among all possibles.
 * If the number returned is 0, no component is preemptable. if it is 1 or more,
 * the oldest_component_preemptable will contain the reference to the preemptable
 * component with the oldest time stamp.
 */
int searchLowerPriority(int current_priority, ComponentListType **oldest_component_preemptable) {
	ComponentListType *componentTemp;
	ComponentListType *componentCandidate;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	int nComp = 0;
	if (!componentList) {
		DEBUG(DEB_LEV_ERR, "In %s no list\n", __func__);
		return OMX_ErrorUndefined;
	}
	componentTemp = componentList;
	componentCandidate = NULL;
	while (componentTemp) {
		if (componentTemp->nGroupPriority > current_priority) {
			nComp++;
		}
		if (nComp>0) {
			if (componentCandidate) {
				if (componentCandidate->timestamp > componentTemp->timestamp) {
					componentCandidate = componentTemp;
				}
			} else {
				componentCandidate = componentTemp;

			}
		}
		componentTemp = componentTemp->next;
	}
	*oldest_component_preemptable = componentCandidate;
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return nComp;
}

/** this function tries to preempt the given component, that has been detected as
 * the candidate by the default policy defined in the OpenMAX spec.
 *
 */
OMX_ERRORTYPE RM_preemptComponent(OMX_COMPONENTTYPE *openmaxStandComp) {
	OMX_STATETYPE state;
    OMX_ERRORTYPE err;
    omx_base_component_PrivateType* omx_base_component_Private = openmaxStandComp->pComponentPrivate;
    internalRequestMessageType message;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
    err = openmaxStandComp->GetState(openmaxStandComp, &state);
	DEBUG(DEB_LEV_PARAMS, "In %s, err = %i, state = %i\n", __func__, (int)err, (int)state);
    if (state == OMX_StateIdle) {
        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp, omx_base_component_Private->callbackData,
            OMX_EventError, OMX_ErrorResourcesLost, 0, NULL);

    	message.messageParam = OMX_StateLoaded;
    	message.pCmdData = NULL;
    	message.messageType = OMX_CommandStateSet;
    	err = omx_base_component_Private->messageHandler(openmaxStandComp, &message);
        if (err != OMX_ErrorNone) {
        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
        }
    }
    if ((state == OMX_StateExecuting) || (state == OMX_StatePause)) {
    	// TODO fill also this section that cover the preemption of a running component
    	// send OMX_ErrorResourcesPreempted
    	// change state to Idle
    	// send OMX_ErrorResourcesLost
    	// change state to Loaded
    }
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function adds the given component to the list of components already allocated.
 */
OMX_ERRORTYPE RM_addComponent(OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentTemp;
	ComponentListType *componentNext;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!componentList) {
		componentList = malloc(sizeof(ComponentListType));
		if (!componentList) {
			DEBUG(DEB_LEV_ERR, "In %s OMX_ErrorInsufficientResources\n", __func__);
			return OMX_ErrorInsufficientResources;
		}
		componentList->openmaxStandComp = openmaxStandComp;
		componentList->next = NULL;
		DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
		return OMX_ErrorNone;
	}
	componentTemp = componentList;
	while(componentTemp->next) {
		componentTemp = componentTemp->next;
	}
	componentNext = malloc(sizeof(ComponentListType));
	if (!componentNext) {
		DEBUG(DEB_LEV_ERR, "In %s OMX_ErrorInsufficientResources\n", __func__);
		return OMX_ErrorInsufficientResources;
	}
	componentTemp->next = componentNext;
	componentNext->next = NULL;
	componentNext->openmaxStandComp = openmaxStandComp;
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function removes from the list the specified component, if it exists.
 */
OMX_ERRORTYPE RM_removeComponent(OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentTemp;
	ComponentListType *componentPrev;
	OMX_BOOL bFound = OMX_FALSE;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!componentList) {
		DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
		return OMX_ErrorUndefined;
	}
	componentTemp = componentList;
	componentPrev = componentList;
	while(componentTemp) {
		if (componentTemp->openmaxStandComp == openmaxStandComp) {
			if (componentTemp == componentList) {
				componentList = componentList->next;
				free(componentTemp);
			} else {
				componentPrev->next = componentTemp->next;
				free(componentTemp);
			}
			 bFound = OMX_TRUE;
			 break;
		} else {
			if (componentTemp != componentList) {
				componentPrev = componentPrev->next;
			}
			componentTemp = componentTemp->next;
		}
	}
	if(!bFound) {
		DEBUG(DEB_LEV_ERR, "In %s, the specified component does not exist\n", __func__);
		return OMX_ErrorComponentNotFound;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/** This function is executed by a component when it changes state from Loaded to Idle.
 * If it return ErrorNone the resource is granted and it can transit to Idle.
 * In case the resource is already busy, the resource manager preempt another component
 * with a lower priority and a oldest time flag if it exists. Differently it returns OMX_ErrorInsufficientResources
 *
 */
OMX_ERRORTYPE RM_getResource(OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentTemp = componentList;
	ComponentListType *componentCandidate;
	omx_base_component_PrivateType* omx_base_component_Private;
	int candidates;
	OMX_ERRORTYPE err;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!componentList) {
		DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
		return OMX_ErrorUndefined;
	}
	if (componentTemp->openmaxStandComp == NULL) {
		// the list is empty
		DEBUG(DEB_LEV_ERR, "In %s, the list is empty\n", __func__);
		return OMX_ErrorComponentNotFound;
	}
	while(componentTemp) {
		if (componentTemp->openmaxStandComp == openmaxStandComp) {
			omx_base_component_Private = (omx_base_component_PrivateType*)componentTemp->openmaxStandComp->pComponentPrivate;
			componentTemp->nGroupPriority = omx_base_component_Private->nGroupPriority;
			componentTemp->timestamp = globalTimestamp;
			globalTimestamp++;
			if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
				noVolumeResourceAllocated++;
				if (noVolumeResourceAllocated>MAX_RUNNING_COMPONENT_VOLUME) {
					candidates = searchLowerPriority(componentTemp->nGroupPriority, &componentCandidate);
					if (candidates) {
						DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s candidates %i winner %x\n", __func__, candidates, (int)componentCandidate->openmaxStandComp);
						err = RM_preemptComponent(componentCandidate->openmaxStandComp);
						if (err != OMX_ErrorNone) {
							DEBUG(DEB_LEV_ERR, "In %s the component cannot be preempted\n", __func__);
							noVolumeResourceAllocated--;
							return OMX_ErrorInsufficientResources;
						}
					} else {
						noVolumeResourceAllocated--;
						DEBUG(DEB_LEV_ERR, "Out of %s\n", __func__);
						return OMX_ErrorInsufficientResources;
					}
				}
			}
			break;
		}
		componentTemp = componentTemp->next;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/** This function is called by a component when it transit from Idle to Loaded and can release
 * its used resource handled by the resource manager
 */
OMX_ERRORTYPE RM_releaseResource(OMX_COMPONENTTYPE *openmaxStandComp){
	ComponentListType *componentTemp = componentList;
	omx_base_component_PrivateType* omx_base_component_Private;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!componentList) {
		DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
		return OMX_ErrorUndefined;
	}
	if (componentTemp->openmaxStandComp == NULL) {
		// the list is empty
		DEBUG(DEB_LEV_ERR, "In %s, the list is empty\n", __func__);
		return OMX_ErrorComponentNotFound;
	}
	while(componentTemp) {
		if (componentTemp->openmaxStandComp == openmaxStandComp) {
			omx_base_component_Private = (omx_base_component_PrivateType*)componentTemp->openmaxStandComp->pComponentPrivate;
			if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
				noVolumeResourceAllocated--;
			}
			break;
		}
		componentTemp = componentTemp->next;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of  %s\n", __func__);
	return OMX_ErrorNone;
}
