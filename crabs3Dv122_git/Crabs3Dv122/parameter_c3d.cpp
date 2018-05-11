#include "parameter_c3d.h"
#include "templates.h"

#include <cstdlib>

/**********************************/

Parameter_Header_c3d::Parameter_Header_c3d() {

}

Parameter_Header_c3d Parameter_Header_c3d::ReadParameterHeaderBlock(FILE* file) {
    Parameter_Header_c3d header;

    readFromFile(&header.parameterBlock, SIZE_8_BIT, file);
    readFromFile(&header.idNumber, SIZE_8_BIT, file);
    readFromFile(&header.numberOfParameters, SIZE_8_BIT, file);
    readFromFile(&header.processorType, SIZE_8_BIT, file);

    return header;
}

bool Parameter_Header_c3d::PrintParameterHeaderToFile(FILE* file) {
    fprintf(file, "Parameter_Block_Start = %d\n", parameterBlock);
    fprintf(file, "ADTech_ID_Number = %d\n", idNumber);
    fprintf(file, "Parameter_512_byte_Block = %d\n", numberOfParameters);
    fprintf(file, "Processor_Type = %d\n", processorType);

    return true;
}

bool Parameter_Header_c3d::PrintParameterHeaderToFile(std::string path) {
    FILE* outputFile;
    outputFile = fopen(path.c_str(), "w");

    fprintf(outputFile, "Parameter_Block_Start = %d\n", parameterBlock);
    fprintf(outputFile, "ADTech_ID_Number = %d\n", idNumber);
    fprintf(outputFile, "Parameter_512_byte_Block = %d\n", numberOfParameters);
    fprintf(outputFile, "Processor_Type = %d\n", processorType);

    fclose(outputFile);

    return true;
}


/**********************************/

bool Group_Parameter_c3d::ReadGroupParameter(FILE* file, Group_Parameter_c3d& parameter, const int flag) {
    bool loop = true;

    readFromFileWithEndian(&parameter.characterNumber, SIZE_8_BIT, file, flag); /* Read characterNumber */
    if(parameter.characterNumber < 0) {
        parameter.characterNumber *= -1;
        parameter.locked = true;
    } else
        parameter.locked = false;

    readFromFileWithEndian(&parameter.parameterID, SIZE_8_BIT, file, flag); /* Read parameterID */
    if(parameter.parameterID > 128)
        parameter.parameterID -= 256;

    parameter.parameterName = (char*)malloc((parameter.characterNumber+1)*sizeof(char));
    for(int i = 0; i < parameter.characterNumber; i++) {
        readFromFileWithEndian(&parameter.parameterName[i], SIZE_8_BIT, file, flag); /* Read parameterName */
        parameter.parameterName[i+1] = '\0';
    }

    parameter.nextParameterStart = 0;
    readFromFileWithEndian(&parameter.nextParameterStart, SIZE_16_BIT, file, flag); /* Read nextParameterStart */

    parameter.byteFormat = 0;
    readFromFileWithEndian(&parameter.byteFormat, SIZE_8_BIT, file, flag); /* Read byteFormat */
    if(parameter.byteFormat == 255)
        parameter.byteFormat = -1;

    parameter.numberOfDimensions = 0;
    readFromFileWithEndian(&parameter.numberOfDimensions, SIZE_8_BIT, file, flag); /* Read numberOfDimensions */

    int dim = 1;
    if(parameter.numberOfDimensions == 0)
        parameter.parameterDimensions[0] = 0;
    else
        for(int i = 0; i < parameter.numberOfDimensions; i++) {
            readFromFileWithEndian(&parameter.parameterDimensions[i], SIZE_8_BIT, file, flag); /* Read parameterDimensions[i] */
            dim *= parameter.parameterDimensions[i];
        }

    switch (parameter.byteFormat) {
    case FORMAT_CHAR:
        parameter.parameterDataChar = (char*)malloc((dim+1)*sizeof(char));
        parameter.parameterDataChar[0] = '\0';
        for(int i = 0; i < dim; i++) {
            readFromFileWithEndian(&parameter.parameterDataChar[i], SIZE_8_BIT, file, flag); /* Read parameterDataChar[i] */
            parameter.parameterDataChar[i+1] = '\0';
        }
        break;
    case FORMAT_BYTE:
        parameter.parameterDataByte = (short int*)malloc((dim+1)*sizeof(short int));
        for(int i = 0; i < dim; i++) {
            parameter.parameterDataByte[i] = 0;
            readFromFileWithEndian(&parameter.parameterDataByte[i], SIZE_8_BIT, file, flag); /* Read parameterDataByte[i] */
        }
        break;
    case FORMAT_INT_16:
        parameter.parameterDataInt_16 = (short int*)malloc((dim+1)*sizeof(short int));
        for(int i = 0; i < dim; i++) {
            parameter.parameterDataInt_16[i] = 0;
            readFromFileWithEndian(&parameter.parameterDataInt_16[i], SIZE_16_BIT, file, flag); /* Read parameterDataInt_16[i] */
        }
        break;
    case FORMAT_FLOAT:
        parameter.parameterDataReal = (float*)malloc((dim+1)*sizeof(float));
        for(int i = 0; i < dim; i++) {
            parameter.parameterDataReal[i] = 0;
            readFromFileWithEndian(&parameter.parameterDataReal[i], SIZE_32_BIT, file, flag); /* Read parameterDataReal[i] */
        }
        break;
    default:
        break;
    }

    readFromFileWithEndian(&parameter.descriptionSize, SIZE_8_BIT, file, flag); /* Read descriptionSize */
    if(parameter.descriptionSize == 0) {
        parameter.description = (char*)malloc(1*sizeof(char));
        parameter.description[0] = '\0';
    } else {
        parameter.description = (char*)malloc((parameter.descriptionSize)*sizeof(char));
        for(int i = 0; i < parameter.descriptionSize; i++) {
            readFromFileWithEndian(&parameter.description[i], SIZE_8_BIT, file, flag); /* Read description */
            parameter.description[i+1] = '\0';
        }
    }

    /* Check if this is the Last group */
    if(parameter.nextParameterStart == 0)
        loop = false;

    return loop;
}


/**********************************/

Group_c3d::Group_c3d() {

}

bool Group_c3d::ReadGroup(FILE* file, Group_c3d& group, const int flag, const bool isGroup) {

    bool loop = true;

    if(isGroup == true) {
        group.parameter = (Group_Parameter_c3d*)malloc(MAX_GROUP_PARAMETER_SIZE*sizeof(Group_Parameter_c3d));
        group.parameterSize = 0; /* Set parameter size for this group to 0. */

        readFromFileWithEndian(&group.characterNumber, SIZE_8_BIT, file, flag); /* Read characterNumber */
        if(group.characterNumber < 0) {
            group.characterNumber *= -1;
            group.locked = true;
        } else
            group.locked = false;

        readFromFileWithEndian(&group.groupID, SIZE_8_BIT, file, flag); /* Read groupID */
        if(group.groupID > 128)
            group.groupID -= 256;

        group.groupName = (char*)malloc((group.characterNumber+1)*sizeof(char));
        for(int i = 0; i < group.characterNumber; i++) {
            readFromFileWithEndian(&group.groupName[i], SIZE_8_BIT, file, flag); /* Read groupName */
            group.groupName[i+1] = '\0';
        }

        group.nextGroupStart = 0;
        readFromFileWithEndian(&group.nextGroupStart, SIZE_16_BIT, file, flag); /* Read nextGroupStart */

        readFromFileWithEndian(&group.descriptionSize, SIZE_8_BIT, file, flag); /* Read descriptionSize */
        if(group.descriptionSize == 0) {
            group.description = (char*)malloc(1*sizeof(char));
            group.description[0] = '\0';
        } else {
            group.description = (char*)malloc((group.descriptionSize)*sizeof(char));
            for(int i = 0; i < group.descriptionSize; i++) {
                readFromFileWithEndian(&group.description[i], SIZE_8_BIT, file, flag); /* Read description */
                group.description[i+1] = '\0';
            }
        }

        /* Check if this is the Last group */
        if(group.nextGroupStart == 0)
            loop = false;

    } else {
        loop = group.parameter->ReadGroupParameter(file, group.parameter[group.parameterSize], flag);
        group.parameterSize++;
    }

    return loop;
}

Group_c3d::~Group_c3d() {

}

/**********************************/

Parameter_c3d::Parameter_c3d() {

}

Parameter_c3d Parameter_c3d::ReadParameterHeaderBlock(FILE* file) {
    Parameter_c3d parameter;

    parameter.header = parameter.header.ReadParameterHeaderBlock(file);

    return parameter;
}

Parameter_c3d Parameter_c3d::ReadGroupParameterBlock(FILE* file, Parameter_c3d parameter, const int flag) {
    Parameter_c3d parameterBuf = parameter;

    parameterBuf.groupSize = 0; /* Set the real groupSize to zero */

    short int numChar = 0;
    short int parId = 0;

    parameterBuf.group = (Group_c3d*)malloc((parameterBuf.groupSize+1)*sizeof(Group_c3d));
    bool loop = true;
    while(loop) {
        readFromFileWithEndian(&numChar, SIZE_8_BIT, file, flag); /* Read number of Characters */
        readFromFileWithEndian(&parId, SIZE_8_BIT, file, flag); /* Read Id Number */
        if(parId > 128)
            parId -= 256;

        fseek(file, -2, SEEK_CUR); /* Return back to char number (correct the reading procedure) */

        /* Check if is Group or Parameter pass the correct flag */
        if( parId < 0) {
            loop = parameterBuf.group[parameterBuf.groupSize].ReadGroup(file, parameterBuf.group[parameterBuf.groupSize], flag, true);
            parameterBuf.groupSize++;
            parameterBuf.group = (Group_c3d*)realloc(parameterBuf.group, (parameterBuf.groupSize+1)*sizeof(Group_c3d));
        } else if ( parId > 0) {
            loop = parameterBuf.group[parId-1].ReadGroup(file, parameterBuf.group[parId-1], flag, false);
        } else if( numChar == 0 || parId == 0)
            loop = false;
    }
    short int buf = 0;
    while (buf == 0)
        readFromFileWithEndian(&buf, SIZE_8_BIT, file, flag);

    fseek(file, -1, SEEK_CUR);

    return parameterBuf;
}

bool Parameter_c3d::PrintToFile(std::string path) {
    FILE* outputFile;
    outputFile = fopen(path.c_str(), "w");

    header.PrintParameterHeaderToFile(outputFile);

    for (int i = 0; i < groupSize; i++) {
        fprintf(outputFile, "Group_Name = %s\n", group[i].Name().c_str());
        fprintf(outputFile, "Group_ID = %d\n", group[i].ID());
        if(group[i].IsLocked()) {
            fprintf(outputFile, "IsLocked = TRUE\n");
        } else {
            fprintf(outputFile, "IsLocked = FALSE\n");
        }
        fprintf(outputFile, "Description = %s\n\n", group[i].Description().c_str()); //Group Description

        for(int j = 0; j < group[i].ParameterSize(); j++) {
            fprintf(outputFile, "\t\tParameter_Name = %s\n", group[i].Parameter(j).Name().c_str()); //Parameter Name
            fprintf(outputFile, "\t\tParameter_ID = %d\n", group[i].Parameter(j).ID()); //Parameter ID

            //Check if Parameter is Lockes
            if(group[i].Parameter(j).IsLocked()) {
                fprintf(outputFile, "\t\tIsLocked = TRUE\n");
            } else {
                fprintf(outputFile, "\t\tIsLocked = FALSE\n");
            }

            //Parameter Format
            switch(group[i].Parameter(j).Format()) {
            case FORMAT_CHAR: {
                fprintf(outputFile, "\t\tByte_Format = CHAR\n");
                break;
            }
            case FORMAT_BYTE: {
                fprintf(outputFile, "\t\tByte_Format = BYTE\n");
                break;
            }
            case FORMAT_INT_16: {
                fprintf(outputFile, "\t\tByte_Format = INT_16_BIT\n");
                break;
            }
            case FORMAT_FLOAT: {
                fprintf(outputFile, "\t\tByte_Format = FLOAT (REAL)\n");
                break;
            }
            default:
                break;
            }

            //Number of Dimensions
            fprintf(outputFile, "\t\tNumber_Of_Dimensions = %d\n",group[i].Parameter(j).DimensionSize());

            //Dimensions Size
            int dim_size = group[i].Parameter(j).Dimension(0);
            fprintf(outputFile, "\t\tDimension_Sizes = %d", group[i].Parameter(j).Dimension(0));
            for(int dim = 1; dim <  group[i].Parameter(j).DimensionSize(); dim++) {
                fprintf(outputFile, " x %d", group[i].Parameter(j).Dimension(dim));
                dim_size *= group[i].Parameter(j).Dimension(dim);
            }
            fprintf(outputFile, "\n");

            //Parameter Data
            fprintf(outputFile, "\t\tParameter_Data: ");
            switch(group[i].Parameter(j).Format()) {
            case FORMAT_CHAR: {
                fprintf(outputFile, "%s\n", group[i].Parameter(j).ParameterChar().c_str());
                if(group[i].Parameter(j).ParameterChar() == " ") {
                    fprintf(outputFile, "\n");
                }
                break;
            }
            case FORMAT_BYTE: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%5d\n", group[i].Parameter(j).ParameterByte(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %5d\n", group[i].Parameter(j).ParameterByte(dim));
                    }
                }
                break;
            }
            case FORMAT_INT_16: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%5d\n", group[i].Parameter(j).ParameterInt_16(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %5d\n", group[i].Parameter(j).ParameterInt_16(dim));
                    }
                }
                break;
            }
            case FORMAT_FLOAT: {
                for(int dim = 0; dim < dim_size; dim++) {
                    if(dim == 0) {
                        fprintf(outputFile,"%8.4f\n", group[i].Parameter(j).ParameterReal(dim));
                    } else {
                       fprintf(outputFile, "\t\t                %8.4f\n", group[i].Parameter(j).ParameterReal(dim));
                    }
                }
                break;
            }
            default:
                fprintf(outputFile, "\n");
                break;
            }

            fprintf(outputFile, "\t\tDescription = %s\n\n", group[i].Parameter(j).Description().c_str()); //Parameter Description
        }
    }

    fclose(outputFile);

    return true;
}

Parameter_c3d::~Parameter_c3d() {

}

/**********************************/

/*******************************/
/* PARAMETER STRUCTURE CLASSES */
/*******************************/

/***************/
/*   1.TRIAL   */
/***************/

Trial Trial::SetTrial(Parameter_c3d parameter) {
    //Set Default Values
    Trial trial;

    trial.Actual_Start_Field = 0;
    trial.Actual_Start_Field = 0;
    trial.Video_Rate_Divider = 0;
    trial.Camera_Rate = 0.0;

    trial.Date[0] = 0;
    trial.Date[1] = 0;
    trial.Date[2] = 0;

    trial.Time[0] = 0;
    trial.Time[1] = 0;
    trial.Time[2] = 0;

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "TRIAL") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "ACTUAL_START_FIELD") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial.Actual_Start_Field = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "ACTUAL_END_FIELD"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial.Actual_End_Field = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "VIDEO_RATE_DIVIDER"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            trial.Video_Rate_Divider = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "CAMERA_RATE"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            trial.Camera_Rate = parameter.Group(i).Parameter(j).ParameterReal(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "DATE"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 3)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16) {
                            trial.Date[0] = parameter.Group(i).Parameter(j).ParameterReal(0);
                            trial.Date[1] = parameter.Group(i).Parameter(j).ParameterReal(1);
                            trial.Date[2] = parameter.Group(i).Parameter(j).ParameterReal(2);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "TIME"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 3)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16) {
                            trial.Time[0] = parameter.Group(i).Parameter(j).ParameterReal(0);
                            trial.Time[1] = parameter.Group(i).Parameter(j).ParameterReal(1);
                            trial.Time[2] = parameter.Group(i).Parameter(j).ParameterReal(2);
                        }
                }

            }
        }
    }

    return trial;
}

/******************/
/*   2.SUBJECTS   */
/******************/

Subjects Subjects::SetSubjects(Parameter_c3d parameter) {
    Subjects subjects;

    //integers
    subjects.is_static = 0;
    subjects.namesSize = 0;
    subjects.model_paramsSize = 0;
    subjects.uses_prefixes = 0;
    subjects.label_prefixesSize = 0;
    subjects.used = 0;
    subjects.marker_setsSize = 0;
    subjects.display_setsSize = 0;
    subjects.modelsSize = 0;

    //strings
    subjects.names = new std::string[1];
    subjects.names[0] = "";

    subjects.model_params = new std::string[1];
    subjects.model_params[0] = "";

    subjects.label_prefixes = new std::string[1];
    subjects.label_prefixes[0] = "";

    subjects.marker_sets = new std::string[1];
    subjects.marker_sets[0] = "";

    subjects.display_sets = new std::string[1];
    subjects.display_sets[0] = "";

    subjects.models = new std::string[1];
    subjects.models[0] = "";

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "SUBJECTS") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "IS_STATIC") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects.is_static = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "NAMES"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.namesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.names = new std::string[subjects.namesSize + 1];

                            for(int k = 0; k < subjects.namesSize; k++)
                                subjects.names[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MODEL_PARAMS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.model_paramsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.model_params = new std::string[subjects.model_paramsSize + 1];

                            for(int k = 0; k < subjects.model_paramsSize; k++)
                                subjects.model_params[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "USES_PREFIXES") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects.uses_prefixes = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABEL_PREFIXES"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.label_prefixesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.label_prefixes = new std::string[subjects.label_prefixesSize + 1];

                            for(int k = 0; k < subjects.label_prefixesSize; k++)
                                subjects.label_prefixes[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "USED") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            subjects.used = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "MARKER_SETS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.marker_setsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.marker_sets = new std::string[subjects.marker_setsSize + 1];

                            for(int k = 0; k < subjects.marker_setsSize; k++)
                                subjects.marker_sets[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DISPLAY_SETS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.display_setsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.display_sets = new std::string[subjects.display_setsSize + 1];

                            for(int k = 0; k < subjects.marker_setsSize; k++)
                                subjects.display_sets[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MODELS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            subjects.modelsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            subjects.models = new std::string[subjects.modelsSize + 1];

                            for(int k = 0; k < subjects.modelsSize; k++)
                                subjects.models[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                }
            }
        }
    }

    return subjects;
}

void Subjects::CleanMemory(Subjects& subjects) {
    delete [] subjects.names;
    delete [] subjects.model_params;
    delete [] subjects.label_prefixes;
    delete [] subjects.marker_sets;
    delete [] subjects.display_sets;
    delete [] subjects.models;
}

/***************/
/*   3.POINT   */
/***************/

Point Point::SetPoint(Parameter_c3d parameter) {
    Point point;

    //Used
    point.used = 0;
    //Scale
    point.scale = 0.0;
    //Rate
    point.rate = 0.0;
    //Data Start
    point.data_start = 0;
    //Frames
    point.frames = 0;

    //Labels
    point.labelsSize = 0;
    point.labels = new std::string[1];
    point.labels[0] = "";

    //Description
    point.descriptionsSize = 0;
    point.descriptions = new std::string[1];
    point.descriptions[0] = "";

    //Units
    point.units = new std::string[1];
    point.units[0] = "";

    //Initial Command
    point.initial_command = new std::string[1];
    point.initial_command[0] = "";

    //X Screen
    point.x_screen = new std::string[1];
    point.x_screen[0] = "";

    //Y Screen
    point.y_screen = new std::string[1];
    point.y_screen[0] = "";

    //Movie_Delay
    point.movie_delay = 0.0;

    //Labels2
    point.labels2Size = 0;
    point.labels2 = new std::string[1];
    point.labels2[0] = "";

    //Description2
    point.descriptions2Size = 0;
    point.descriptions2 = new std::string[1];
    point.descriptions2[0] = "";

    //Type Groups
    point.type_groupsSize = 0;
    point.type_groups = new std::string[1];
    point.type_groups[0] = "";

    //Angles
    point.anglesSize = 0;
    point.angles = new std::string[1];
    point.angles[0] = "";

    //Angles Units
    point.angle_units = new std::string[1];
    point.angle_units[0] = "";

    //Scalars
    point.scalarsSize = 0;
    point.scalars = new std::string[1];
    point.scalars[0] = "";

    //Scalar Units
    point.scalar_units = new std::string[1];
    point.scalar_units[0] = "";

    //Powers
    point.powersSize = 0;
    point.powers = new std::string[1];
    point.powers[0] = "";

    //Power Units
    point.power_units = new std::string[1];
    point.power_units[0] = "";

    //Forces
    point.forcesSize = 0;
    point.forces = new std::string[1];
    point.forces[0] = "";

    //Force Units
    point.force_units = new std::string[1];
    point.force_units[0] = "";

    //Moments
    point.momentsSize = 0;
    point.moments = new std::string[1];
    point.moments[0] = "";

    //Moment Units
    point.moment_units = new std::string[1];
    point.moment_units[0] = "";

    //Reactions
    point.reactionsSize = 0;
    point.reactions = new std::string[1];
    point.reactions[0] = "";/**/

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "POINT") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "USED") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point.used = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALE") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point.scale = parameter.Group(i).Parameter(j).ParameterReal(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "RATE") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point.rate = parameter.Group(i).Parameter(j).ParameterReal(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "DATA_START") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point.data_start = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "FRAMES") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_INT_16)
                            point.frames = parameter.Group(i).Parameter(j).ParameterInt_16(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABELS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.labelsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.labels = new std::string[point.labelsSize];

                            for(int k = 0; k < point.labelsSize; k++)
                                point.labels[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DESCRIPTIONS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.descriptionsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.descriptions = new std::string[point.descriptionsSize];

                            for(int k = 0; k < point.descriptionsSize; k++)
                                point.descriptions[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "INITIAL_COMMAND"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.initial_command[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "X_SCREEN"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.x_screen[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "Y_SCREEN"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.y_screen[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOVIE_DELAY") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_FLOAT)
                            point.movie_delay = parameter.Group(i).Parameter(j).ParameterReal(0);
                } else if(parameter.Group(i).Parameter(j).Name() == "LABELS2"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.labels2Size = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.labels2 = new std::string[point.labels2Size];

                            for(int k = 0; k < point.labels2Size; k++)
                                point.labels2[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "DESCRIPTIONS2"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.descriptions2Size = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.descriptions2 = new std::string[point.descriptions2Size];

                            for(int k = 0; k < point.descriptions2Size; k++)
                                point.descriptions2[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "TYPE_GROUPS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.type_groupsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.type_groups = new std::string[point.type_groupsSize];

                            for(int k = 0; k < point.type_groupsSize; k++)
                                point.type_groups[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "ANGLES"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.anglesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.angles = new std::string[point.anglesSize];

                            for(int k = 0; k < point.anglesSize; k++)
                                point.angles[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "ANGLE_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.angle_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALARS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.scalarsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.scalars = new std::string[point.scalarsSize];

                            for(int k = 0; k < point.scalarsSize; k++)
                                point.scalars[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "SCALAR_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.scalar_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "POWERS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.powersSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.powers = new std::string[point.powersSize];

                            for(int k = 0; k < point.powersSize; k++)
                                point.powers[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "POWER_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.power_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "FORCES"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.forcesSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.forces = new std::string[point.forcesSize];

                            for(int k = 0; k < point.forcesSize; k++)
                                point.forces[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "FORCE_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.force_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOMENTS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.momentsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.moments = new std::string[point.momentsSize];

                            for(int k = 0; k < point.momentsSize; k++)
                                point.moments[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "MOMENT_UNITS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {
                            point.moment_units[0] = parameter.Group(i).Parameter(j).ParameterChar();
                        }
                } else if(parameter.Group(i).Parameter(j).Name() == "REACTIONS"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR) {

                            point.reactionsSize = wordCounter(parameter.Group(i).Parameter(j).ParameterChar().c_str());
                            point.reactions = new std::string[point.reactionsSize];

                            for(int k = 0; k < point.reactionsSize; k++)
                                point.reactions[k] = wordReturn(parameter.Group(i).Parameter(j).ParameterChar().c_str(), k+1);
                        }
                }
            }
        }
    }

    return point;
}

float Point::MultiplierForMeters(Point point) {

    if(point.Units() == "mm" || point.Units() == "MM" || point.Units() == "Mm" || point.Units() == "mM") {
        return 0.001;
    } else if(point.Units() == "cm" || point.Units() == "CM" || point.Units() == "Cm" || point.Units() == "cM") {
        return 0.01;
    } else if (point.Units() == "dm" || point.Units() == "Dm" || point.Units() == "DM" || point.Units() == "dM") {
        return 0.1;
    } else if (point.Units() == "m" || point.Units() == "M") {
        return 1.0;
    } else if (point.Units() == "km" || point.Units() == "Km" || point.Units() == "KM" || point.Units() == "kM") {
        return 1000.0;
    }

    return 1.0;

}

void Point::CleanMemory(Point& point) {
    delete [] point.labels;
    delete [] point.descriptions;
    delete [] point.units;
    delete [] point.initial_command;
    delete [] point.x_screen;
    delete [] point.y_screen;
    delete [] point.labels2;
    delete [] point.descriptions2;
    delete [] point.type_groups;
    delete [] point.angle_units;
    delete [] point.scalars;
    delete [] point.scalar_units;
    delete [] point.powers;
    delete [] point.power_units;
    delete [] point.forces;
    delete []  point.force_units;
    delete [] point.moments;
    delete [] point.moment_units;
    delete [] point.reactions;
}

/**********************/
/*   8.MANUFACTURER   */
/**********************/

Manufacturer Manufacturer::SetManufacturer(Parameter_c3d parameter) {
    Manufacturer manufacturer;

    manufacturer.company = new std::string[1];
    manufacturer.software = new std::string[1];
    manufacturer.version = new std::string[1];

    manufacturer.company[0] = "";
    manufacturer.software[0] = "";
    manufacturer.version[0] = "";

    for(int i = 0; i < parameter.GroupSize(); i++) {
        if(parameter.Group(i).Name() == "MANUFACTURER") {
            for(int j = 0; j < parameter.Group(i).ParameterSize(); j++) {

                if(parameter.Group(i).Parameter(j).Name() == "COMPANY") {
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer.company[0] = parameter.Group(i).Parameter(j).ParameterChar();
                } else if(parameter.Group(i).Parameter(j).Name() == "SOFTWARE"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer.software[0] = parameter.Group(i).Parameter(j).ParameterChar();
                } else if(parameter.Group(i).Parameter(j).Name() == "VERSION" || parameter.Group(i).Parameter(j).Name() == "VERSION_LABEL"){
                    if(parameter.Group(i).Parameter(j).DimensionSize() > 0)
                        if(parameter.Group(i).Parameter(j).Format() == FORMAT_CHAR)
                            manufacturer.version[0] = parameter.Group(i).Parameter(j).ParameterChar();
                }
            }

        }
    }

    return manufacturer;
}

void Manufacturer::CleanMemory(Manufacturer& manufacturer) {
    delete [] manufacturer.company;
    delete [] manufacturer.software;
    delete [] manufacturer.version;
}


