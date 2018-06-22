ViStatus _VI_FUNC rsspecan_RestoreGsmLimitLines (ViSession instrSession)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckErr (rsspecan_SetAttributeViString (instrSession, "", RSSPECAN_ATTR_GSM_REST, ""));
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot State
 * Purpose:     This function activates the extended slot configuration.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotState (ViSession instrSession,
                                                          ViBoolean state)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STATE, state),
        2, "State");
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot
 * Purpose:     This function configures the GSM Extended Slot for Multi 
 *              Slot measurements. These values are standard specific
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlot (ViSession instrSession,
                                                             ViInt32 standard,
                                                             ViInt32 triggerReference,
                                                             ViInt32 referenceMidable,
                                                             ViBoolean absoluteLevelState,
                                                             ViReal64 limitBase1,
                                                             ViReal64 limitBase2)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_STANDARD, standard),
        2, "Standard");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_TRIG_REFERENCE, triggerReference),
        3, "Trigger Reference");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MIDAMBLE_REFERENCE, referenceMidable),
        4, "Reference Midable");
    if (standard >= RSSPECAN_VAL_GSM_EXTSLOT_GSM900)
    {
        viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_STATE, absoluteLevelState),
            5, "Absolute Level State");
        if (absoluteLevelState == VI_TRUE)
        {
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "BAS1", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_VALUE, limitBase1),
            6, "Limit Base 1");
            viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, "BAS2", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_ABS_LIMIT_VALUE, limitBase2),
            7, "Limit Base 2");
        }
    }   
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}

/*****************************************************************************
 * Function:    Configure GSM Extended Slot Common Settings
 * Purpose:     This function configures the GSM Extended Slot for Multi 
 *              Slot measurements. These configurations are common for all 
 *              standards.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotCommonSettings (ViSession instrSession,
                                                     ViBoolean longSlots,
                                                     ViInt32 arraySize,
                                                     ViInt32 longSlotsValues[],
                                                     ViBoolean onlyOneFrame)
{
    ViStatus    error = VI_SUCCESS;

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    if (rsspecan_invalidViInt32Range (arraySize, 2, 2) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Array Size");
    
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LONG_SLOT, longSlots),
        2, "Long Slots");
    viCheckErr (viPrintf (instrSession, "CONF:ECON:LSL:VAL %ld,%ld\n", longSlotsValues[0], longSlotsValues[1]));
    checkErr( rsspecan_CheckStatus (instrSession)); 
    viCheckParm (rsspecan_SetAttributeViBoolean (instrSession, "", RSSPECAN_ATTR_GSM_EXTENDED_SLOT_OFRAME, onlyOneFrame),
        3, "Only One Frame");
    
Error:
    (void) Rs_UnlockSession(instrSession, VI_NULL);
    return error;
}


/*****************************************************************************
 * Function:    Configure GSM Extended Slot Parameters
 * Purpose:     This function configures the slot specific parameters for 
 *              GSM Extended Slot for Multi Slot measurements.
 *****************************************************************************/
ViStatus _VI_FUNC rsspecan_ConfigureGsmExtendedSlotParameters (ViSession instrSession,
                                                               ViInt32 slotNumber,
                                                               ViInt32 modulation,
                                                               ViInt32 referenceLevelMode,
                                                               ViReal64 referenceLevelValue,
                                                               ViInt32 limitLineCtrlLevel,
                                                               ViString limitLineLower,
                                                               ViString limitLineUpper)
{
    ViStatus    error = VI_SUCCESS;
    ViChar      buffer[RSSPECAN_IO_BUFFER_SIZE] = "";

    checkErr( Rs_LockSession (instrSession, VI_NULL)); 

    if (rsspecan_invalidViInt32Range (slotNumber, 0, 7) == VI_TRUE)
        viCheckParm (RS_ERROR_INVALID_PARAMETER, 2, "Slot Number");
    
    sprintf (buffer, "SL%ld", slotNumber);
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_MODULATION, modulation),
        3, "Modulation");
    viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_MODE, referenceLevelMode),
        4, "Reference Level Mode");
    viCheckParm (rsspecan_SetAttributeViReal64 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_RLEV_VALUE, referenceLevelValue),
        5, "Reference Level Value");
    if (modulation != RSSPECAN_VAL_EXT_SLOT_MODUL_OFF)   
    {
        viCheckParm (rsspecan_SetAttributeViInt32 (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT_CLEV, limitLineCtrlLevel),
        6, "Limit Line Ctrl Level");
        sprintf (buffer, "SL%ld,Low", slotNumber);
        viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT, limitLineLower),
        7, "Limit Line Lower");
        sprintf (buffer, "SL%ld,Upp", slotNumber); 
        viCheckParm (rsspecan_SetAttributeViString (instrSession, buffer, RSSPECAN_ATTR_GSM_EXTENDED_SLOT_LIMIT, limitLineUpper),
        8, "Limit Line Upper");
    }
    
