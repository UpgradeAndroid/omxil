/**
  @file src/components/symbianoutputstream/omx_symbian_output_stream_wrapper.h
    
  Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).

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

#ifndef __OMX_SYMBIAN_OUTPUT_STREAM_WRAPPER_H__
#define __OMX_SYMBIAN_OUTPUT_STREAM_WRAPPER_H__

#ifdef __cplusplus
extern "C" {
#endif

int create_output_stream(void **output);

int open_output_stream(void *output, int sampleRate, int channels);

int close_output_stream(void *output);

int write_audio_data(void *output, unsigned char *buffer, int length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __OMX_SYMBIAN_OUTPUT_STREAM_WRAPPER_H__
