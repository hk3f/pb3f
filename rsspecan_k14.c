                                                                      ViBoolean timeStamp,
                                                                      ViInt32 frameCount,
                                                                      ViBoolean continueFrame)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));
     
    sprintf (buffer, "Win%ld", win); 

    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_SPEM_HDEP, historyDepth),
                                            2, "History Depth");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_SPEM_TIME_STAMP_STATE, timeStamp),
                                            3, "Time Stamp");
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_SPEM_FRAME_COUNT, frameCount),
                                            4, "Frame Count");
    viCheckParm( rsspecan_SetAttributeViBoolean (instrSession, buffer, RSSPECAN_ATTR_SPEM_CONTINUOUS, continueFrame),
                                            5, "Continue Frame");
    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Selected Frame
 * Purpose:  This function selects the frame if no measurement is running.
             During a measurement, the value is fixed to 0 and this command
             is available only if no measurement is running or after a single
             sweep has ended.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramSelectedFrame (ViSession instrSession,
                                                              ViInt32 frame_number)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));
     
    sprintf (buffer, "Win%ld", win); 
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_SPEM_FRAME_SELECT, frame_number),
                                            2, "Frame number");
    
Error:
     Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Spectrogram Clear
 * Purpose:  This function deletes the spectrogram result display and the history buffer.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_SpectrogramClear (ViSession instrSession)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win));
     
    sprintf (buffer, "Win%ld", win); 

    viCheckErr( rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_SPEM_CLEAR, NULL));

    
Error:
    Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram FFT Window
 * Purpose:  This function selects the type of FFT window that you want to
 *           use in realtime mode.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramFFTWindow (ViSession instrSession,
                                                          ViInt32 FFTWindowType)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, 
        RSSPECAN_ATTR_SPEM_WINDOW, &win));
     
    sprintf (buffer, "Win%ld", win); 
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
        RSSPECAN_ATTR_SPEM_FFT_WINDOW, FFTWindowType), 2, "FFT Window Type");
Error:
     Rs_UnlockSession(instrSession, VI_NULL); 
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Marker Frame Number
 * Purpose:  This function defines the frame number of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerFrameNumber (ViSession instrSession,
                                                                  ViInt32 markerNumber,
                                                                  ViInt32 frameNumber)
{
    ViInt32  win = 1;
    ViStatus    error = VI_SUCCESS;
    ViChar   buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (markerNumber, 1, 4) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Marker Number");
    }
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win)); 
    
    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SPEM_MARK_FRAME_NUMBER,
                                                frameNumber), 3, "Frame Number");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Get Spectrogram Marker Frame Number
 * Purpose:  This function returns the frame number of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_GetSpectrogramMarkerFrameNumber (ViSession instrSession,
                                                            ViInt32 markerNumber,
                                                            ViInt32 *frameNumber)
{
    ViInt32  win = 1;   
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (markerNumber, 1, 4) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Marker Number");
    }
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win)); 
    
    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);
    
    viCheckParm( rsspecan_GetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SPEM_MARK_FRAME_NUMBER,
                                                frameNumber), 3, "Frame Number");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function: Configure Spectrogram Marker Search Area
 * Purpose:  This function defines the search area of the specified marker.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureSpectrogramMarkerSearchArea (ViSession instrSession,
                                                                 ViInt32 markerNumber,
                                                                 ViInt32 searchArea)
{
    ViInt32  win = 1; 
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";
    
    checkErr( Rs_LockSession (instrSession, VI_NULL));
    if (rsspecan_invalidViInt32Range (markerNumber, 1, 4) == VI_TRUE)
    {
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Marker Number");
    }
    checkErr (rsspecan_GetAttributeViInt32 (instrSession, NULL, RSSPECAN_ATTR_SPEM_WINDOW, &win)); 
    
    sprintf (buffer, "Win%ld,M%ld",win, markerNumber);
    
    viCheckParm( rsspecan_SetAttributeViInt32 (instrSession, buffer, 
                                                RSSPECAN_ATTR_SPEM_MARKER_SEARCH_AREA,
                                                searchArea), 3, "Search Area");
    
Error:
