/**
  test/components/resource_manager/omxprioritytest.c

  This simple test application tests the functionalities of the simple resource
  manager provided with Bellagio components that implements the basic support defined in OpenMAX
  for resource management.

  Copyright (C) 2010 STMicroelectronics

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

*/

#include "omxprioritytest.h"
#include <string.h>
#include <bellagio/extension_struct.h>

#define MAX_COMPONENTS 5
/* Application private date: should go in the component field (segs...) */

OMX_ERRORTYPE err;

OMX_HANDLETYPE *handle;

OMX_CALLBACKTYPE callbacks = { .EventHandler = rmEventHandler,
                               .EmptyBufferDone = rmEmptyBufferDone,
                               .FillBufferDone = rmFillBufferDone,
};

static void setHeader(OMX_PTR header, OMX_U32 size) {
  OMX_VERSIONTYPE* ver = (OMX_VERSIONTYPE*)(header + sizeof(OMX_U32));
  *((OMX_U32*)header) = size;

  ver->s.nVersionMajor = VERSIONMAJOR;
  ver->s.nVersionMinor = VERSIONMINOR;
  ver->s.nRevision = VERSIONREVISION;
  ver->s.nStep = VERSIONSTEP;
}


int convertStr2Int(char* str) {
	int val = 0;
	int len = strlen(str);
	int i = 0;
	while(i < len) {
		if ((*(str+i)<'0') || (*(str+i)>'9')) {
			return 0;
		}
		val = (val*10) + ((*(str+i))-'0');
		i++;
	}
	return val;
}

void display_help() {
  printf("\n");
  printf("Usage: omxprioritytest OMX_name [-i max_comp]\n");
  printf("\n");
  exit(1);
}

int max_value;
int main(int argc, char** argv) {
	int getMaxValue = 0;
	int flagInputReceived = 0;
	int argn_dec = 1;
	int i;
	int num_of_components;
  OMX_STATETYPE state;
	char* componentName;
	OMX_PRIORITYMGMTTYPE oPriority;

	max_value = 0;
	if(argc < 2){
		display_help();
	} else {
		while (argn_dec < argc) {
			if (*(argv[argn_dec]) == '-') {
				switch (*(argv[argn_dec] + 1)) {
				case 'h':
					display_help();
					break;
				case 'i':
					getMaxValue = 1;
					break;
				default:
					display_help();
				}
			} else {
				if (getMaxValue) {
					max_value = convertStr2Int(argv[argn_dec]);
					if (max_value == 0) {
						display_help();
					}
				} else {
					componentName = malloc(strlen(argv[argn_dec]) * sizeof(char) + 1);
					strcpy(componentName, argv[argn_dec]);
					flagInputReceived = 1;
				}
			}
			argn_dec++;
		}
	}
	if (!flagInputReceived) {
		display_help();
	}
	if (max_value == 0) {
		max_value = MAX_COMPONENTS;
	}
	handle = malloc(sizeof(OMX_HANDLETYPE*) * max_value);
	/* Obtain file descriptor */
	eventSem = malloc(sizeof(tsem_t));
	tsem_init(eventSem, 0);
	bResourceErrorReceived = OMX_FALSE;
	err = OMX_Init();
	if(err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "OMX_Init() failed\n");
		exit(1);
	}
	DEBUG(DEFAULT_MESSAGES, "OMX_Init()\n");

	for (i = 0; i<max_value; i++) {
		err = OMX_GetHandle(&handle[i], componentName, NULL, &callbacks);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "OMX_GetHandle [%i] failed\n", i);
			exit(1);
		}
		DEBUG(DEFAULT_MESSAGES, "OMX_GetHandle() %i\n", i);
	}

	for (i = 0; i<max_value; i++) {
		num_of_components = i;
		err = OMX_SendCommand(handle[i], OMX_CommandPortDisable, 0, 0);
		err = OMX_SendCommand(handle[i], OMX_CommandPortDisable, 1, 0);
		err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateIdle, NULL);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "The component can't go to Idle\n");
			break;
		}
		tsem_down(eventSem);
		if (bResourceErrorReceived) {
		/** the priority of the component which fails to go to idle is reaised
				so taht another component is preempted to leave room to this component */
			DEBUG(DEFAULT_MESSAGES, "The resources are exhausted\n");
			DEBUG(DEFAULT_MESSAGES, "Raising the priority of component %i\n", i);
			setHeader(&oPriority, sizeof(OMX_PRIORITYMGMTTYPE));
			oPriority.nGroupPriority = 1;
			err = OMX_SetParameter(handle[i], OMX_IndexParamPriorityMgmt, &oPriority);
			err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateIdle, NULL);
			tsem_down(eventSem);
			DEBUG(DEFAULT_MESSAGES, "##################################\n");
			DEBUG(DEFAULT_MESSAGES, "The resource manager has operated!\n");
			DEBUG(DEFAULT_MESSAGES, "##################################\n");
			break;
		}
	}

	DEBUG(DEFAULT_MESSAGES, "Dispose the system\n");
	for (i = 0; i<num_of_components; i++) {
		err = OMX_GetState(handle[i], &state);
		if (state == OMX_StateIdle) {
			err = OMX_SendCommand(handle[i], OMX_CommandStateSet, OMX_StateLoaded, NULL);
	        tsem_down(eventSem);
			DEBUG(DEFAULT_MESSAGES, "Component %i sent to Loaded\n", i);
		} else if (state == OMX_StateLoaded) {
			DEBUG(DEFAULT_MESSAGES, "Component %i already loaded\n", i);
		} else {
			DEBUG(DEFAULT_MESSAGES, "Component %i in the wrong state!\n", i);
		}
	}

	for (i = 0; i<max_value; i++) {
		err = OMX_FreeHandle(handle[i]);
		if(err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "OMX_FreeHandle [%i] failed\n", i);
			exit(1);
		}
	}

	err = OMX_Deinit();
	if(err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "OMX_Deinit() failed\n");
		exit(1);
	}
	free(eventSem);
	DEBUG(DEFAULT_MESSAGES, "OMX_Deinit()\n");
	return 0;
}

/* Callbacks implementation */
OMX_ERRORTYPE rmEventHandler(
  OMX_HANDLETYPE hComponent,
  OMX_PTR pAppData,
  OMX_EVENTTYPE eEvent,
  OMX_U32 Data1,
  OMX_U32 Data2,
  OMX_PTR pEventData) {

  if(eEvent == OMX_EventCmdComplete) {
    if (Data1 == OMX_CommandStateSet) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "Volume Component %x State changed in ", (int)hComponent);
      switch ((int)Data2) {
      case OMX_StateInvalid:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateInvalid\n");
        break;
      case OMX_StateLoaded:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateLoaded\n");
        break;
      case OMX_StateIdle:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateIdle\n");
        break;
      case OMX_StateExecuting:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateExecuting\n");
        break;
      case OMX_StatePause:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StatePause\n");
        break;
      case OMX_StateWaitForResources:
        DEBUG(DEB_LEV_SIMPLE_SEQ, "OMX_StateWaitForResources\n");
        break;
      }
      tsem_up(eventSem);
    } else if (Data1 == OMX_CommandPortDisable) {
    	DEBUG(DEB_LEV_SIMPLE_SEQ, "Disabled port %i\n", (int)Data2);
    }
  } else if (eEvent == OMX_EventError) {
  	if (Data1 == OMX_ErrorInsufficientResources) {
  	  	DEBUG(DEB_LEV_SIMPLE_SEQ, "Received error OMX_ErrorInsufficientResources\n");
  	  	bResourceErrorReceived = OMX_TRUE;
        tsem_up(eventSem);
  	} else if (Data1 == OMX_ErrorResourcesLost) {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error OMX_ErrorResourcesLost\n");
  	} else if (Data1 == OMX_ErrorResourcesPreempted) {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error OMX_ErrorResourcesPreempted\n");
  	} else {
  	  	DEBUG(DEFAULT_MESSAGES, "Received error %i\n", (int)Data1);
  	}
  } else if(eEvent == OMX_EventResourcesAcquired) {
	  	DEBUG(DEFAULT_MESSAGES, "Received message OMX_EventResourcesAcquired\n");
  } else {
    DEBUG(DEB_LEV_SIMPLE_SEQ, "Param1 is %i\n", (int)Data1);
    DEBUG(DEB_LEV_SIMPLE_SEQ, "Param2 is %i\n", (int)Data2);
  }
  return OMX_ErrorNone;
}

OMX_ERRORTYPE rmEmptyBufferDone(
		OMX_HANDLETYPE hComponent,
		OMX_PTR pAppData,
		OMX_BUFFERHEADERTYPE* pBuffer) {

	return OMX_ErrorNone;
}

OMX_ERRORTYPE rmFillBufferDone(
		OMX_HANDLETYPE hComponent,
		OMX_PTR pAppData,
		OMX_BUFFERHEADERTYPE* pBuffer) {

	return OMX_ErrorNone;
}
