
//    sessionProperties -> vi = (ViSession) sessionProperties;
    sessionProperties -> io = io;
    checkErr (viSetAttribute (io, VI_ATTR_USER_DATA, (ViAttrState) sessionProperties));
    
    if (gresCount == 0)
    {
        memset (gresources, 0, MAX_NUMBER_OF_SESSIONS * sizeof (TsessionData));
    }
    while ((gresources[resource_idx].session != 0) && (resource_idx < gresCount)) resource_idx++;
    gresources[resource_idx].session = io;
    gresources[resource_idx].properties = sessionProperties;
    gresCount++;
    
    /* --- Default option checking --- */
    sessionProperties -> optionChecking = VI_TRUE;
    
    /* --- Determine number of attributes --- */
    for (numberOfAttr = 0; attrList[numberOfAttr]; numberOfAttr++);
    //for (numberOfAttr = 0; RsAttrPropertiesList[numberOfAttr]; numberOfAttr++);

    sessionProperties -> numberOfAttr = numberOfAttr;

    /* --- Allocate memory for attribute properties list --- */

    if ((attrPropertiesList = (RsAttrProperties *) malloc (sizeof (RsAttrProperties) * numberOfAttr)) == VI_NULL)
        return RS_ERROR_OUT_OF_MEMORY;

    memset (attrPropertiesList, 0, sizeof (RsAttrProperties) * numberOfAttr);

    sessionProperties -> attr = attrPropertiesList;

    /* ---
            Allocate memory for current (cached) value of each attribute and copy statically
            declared attribute properties into session-based allocated memory space.
    --- */

    for (idx = 0; idx < (ViInt32) numberOfAttr; idx++)
        {
        memcpy (&sessionProperties -> attr[idx], attrList[idx], sizeof (RsAttrProperties));
        //memcpy (&sessionProperties -> attr[idx], RsAttrPropertiesList[idx], sizeof (RsAttrProperties));

        switch (sessionProperties -> attr[idx].dataType)
            {
            case RS_VAL_INT32:
                data_size = RS_VAL_INT32_SIZE;
                break;
            case RS_VAL_REAL64:
                data_size = RS_VAL_REAL64_SIZE;
                break;
            case RS_VAL_STRING:
                data_size = RS_VAL_STRING_SIZE;
                break;
            case RS_VAL_ADDR:
                data_size = RS_VAL_ADDR_SIZE;
                break;
            case RS_VAL_SESSION:
                data_size = RS_VAL_SESSION_SIZE;
                break;
            case RS_VAL_BOOLEAN:
                data_size = RS_VAL_BOOLEAN_SIZE;
                break;
            case RS_VAL_UNKNOWN_TYPE:
            default:
                data_size = RS_VAL_UNKNOWN_TYPE_SIZE;
            }

        p2value = malloc (data_size);
        
        if (p2value == VI_NULL)
            return RS_ERROR_OUT_OF_MEMORY;

        memset (p2value, 0, data_size);

        sessionProperties -> attr[idx].p2value = p2value;
        }

    /* --- Pass value to RS_ATTR_SPECIFIC_DRIVER_PREFIX --- */

    checkErr (Rs_GetAttributeIndex (io, RS_ATTR_SPECIFIC_DRIVER_PREFIX, &idx));

    strncpy ((ViString)sessionProperties -> attr[idx].p2value, specificPrefix, RS_MAX_MESSAGE_BUF_SIZE - 1);

    /* --- Parse option string and optionally sets the initial state of the following session attributes --- */

    /* Tokenize option string and search for known options */

    if (optionsString)
        if (strlen (optionsString) > 0)
            {
            if ((driver_setup = (ViChar*) malloc (strlen (optionsString)+1)) == VI_NULL)
                return RS_ERROR_OUT_OF_MEMORY;

            memset (driver_setup, 0, strlen (optionsString)+1);

            p2buf = (ViChar *)optionsString;
            p2buf = strtok (p2buf, ",");

            while (p2buf)
            {
                /* RangeCheck */
                if (strncmp (p2buf, "RangeCheck", 10) == 0)
                {
                    p2temp = strstr (p2buf, "=");
            
                    if (p2temp)
                        {
                        checkErr (Rs_GetAttributeIndex (io, RS_ATTR_RANGE_CHECK, &idx));
                
                        if ((strstr (p2temp, "1")       != NULL) || 
                            (strstr (p2temp, "True")    != NULL) ||
                            (strstr (p2temp, "VI_TRUE") != NULL))
                            {
                            boolean = VI_TRUE;
                            *(ViBoolean *)sessionProperties -> attr[idx].p2value = boolean;
                            }
                        else
                            if ((strstr (p2temp, "0")        != NULL) || 
                                (strstr (p2temp, "False")    != NULL) || 
                                (strstr (p2temp, "VI_FALSE") != NULL))
                                {
                                boolean = VI_FALSE;
                                *(ViBoolean *)sessionProperties -> attr[idx].p2value = boolean;
                                }
                        }
            
                    p2buf = strtok (NULL, ",");
                    continue;
                }
        
                /* QueryInstrStatus */
                if (strncmp (p2buf, "QueryInstrStatus", 16) == 0)
                {
                    p2temp = strstr (p2buf, "=");
            
                    if (p2temp)
                        {
                        checkErr (Rs_GetAttributeIndex (io, RS_ATTR_QUERY_INSTR_STATUS, &idx));
                
                        if ((strstr (p2temp, "1")       != NULL) || 
                            (strstr (p2temp, "True")    != NULL) || 
                            (strstr (p2temp, "VI_TRUE") != NULL))
                            {
                            boolean = VI_TRUE;
                            *(ViBoolean *)sessionProperties -> attr[idx].p2value = boolean;
                            }
                        else
                            if ((strstr (p2temp, "0")        != NULL) ||
                                (strstr (p2temp, "False")    != NULL) || 
                                (strstr (p2temp, "VI_FALSE") != NULL))
                                {
                                boolean = VI_FALSE;
                                *(ViBoolean *)sessionProperties -> attr[idx].p2value = boolean;
                                }
                        }
            
                    p2buf = strtok (NULL, ",");
                    continue;
                }
        
                /* Cache */
