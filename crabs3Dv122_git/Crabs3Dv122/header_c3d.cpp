#include "header_c3d.h"
#include "templates.h"

using std::string;
using std::vector;

Header_c3d::Header_c3d()
{
    parameterBlock = 0;
    idNumber = 0;
    pointsNumber = 0;
    analogNumber = 0;
    firstFrame = 0;
    lastFrame = 0;
    maximumInterpolationGap = 0;
    scaleFactor = 0.0;
    dataStart = 0;
    analogPerFrame = 0;
    frameRate = 0;
    for(int i = 0; i < futureBlock_1_size; i++)
        futureUseBlock_1.push_back(0);
    keyValue_1 = 0;
    firstLabelRange = 0;
    keyValue_2 = 0;
    eventTimeSize = 0;
    futureUseBlock_2 = 0;
    for(int i = 0; i < eventTime_size; i++)
        eventTimeInSeconds.push_back(0.0);
    for(int i = 0; i < displayFlag_size; i++)
        eventDisplayFlags.push_back(0);
    futureUseBlock_3 = 0;
    for(int i = 0; i < eventLabel_size; i++)
        eventLabels.push_back(string(4, '\0'));
    for(int i = 0; i < futureBlock_4_size; i++)
        futureUseBlock_4.push_back(0);
}

/* Read the 512-bytes Header block */
Header_c3d Header_c3d::ReadHeaderBlock(FILE* file) {
    Header_c3d header;

    readFromFile(&header.parameterBlock, SIZE_8_BIT, file); /* Read parameterBlock -> 1-byte */
    /*
     * If Parameter Block Byte is 1 this means the file has no Header Section
     * and there is no point to continue reading.
     */
    if(header.parameterBlock == 1) {
        fseek(file, 0, SEEK_SET);
        return header;
    }

    readFromFile(&header.idNumber, SIZE_8_BIT, file); /* Read  idNumber -> 1-byte */
    readFromFile(&header.pointsNumber, SIZE_16_BIT, file); /* Read pointsNumber -> 2-bytes */
    readFromFile(&header.analogNumber, SIZE_16_BIT, file); /* Read analogNumber -> 2-bytes */
    readFromFile(&header.firstFrame, SIZE_16_BIT, file); /* Read firstFrame -> 2-bytes */
    readFromFile(&header.lastFrame, SIZE_16_BIT, file); /* Read lastFrame -> 2-bytes */
    readFromFile(&header.maximumInterpolationGap, SIZE_16_BIT, file); /* Read maximuInterpolationGap -> 2-bytes */
    readFromFile(&header.scaleFactor, SIZE_32_BIT, file); /* Read scaleFactor -> 4-bytes */
    readFromFile(&header.dataStart, SIZE_16_BIT, file); /* Read dataStart -> 2-bytes */
    readFromFile(&header.analogPerFrame, SIZE_16_BIT, file); /* Read analogPerFrame -> 2-bytes */
    readFromFile(&header.frameRate, SIZE_32_BIT, file); /* Read frameRate -> 4-bytes */

    for(auto iter : header.futureUseBlock_1)
        readFromFile(&iter, SIZE_16_BIT, file); /* Read futureUseBlock_1[i] -> 270-bytes */

    readFromFile(&header.keyValue_1, SIZE_16_BIT, file); /* Read keyValue_1 -> 2-bytes */
    readFromFile(&header.firstLabelRange, SIZE_16_BIT, file); /* Read firstLabelRange -> 2-bytes */
    readFromFile(&header.keyValue_2, SIZE_16_BIT, file); /* Read keyValue_2 -> 2-bytes */
    readFromFile(&header.eventTimeSize, SIZE_16_BIT, file); /* Read eventTimeSize -> 2-bytes */
    readFromFile(&header.futureUseBlock_2, SIZE_16_BIT, file); /* Read futureUseBlock_2 -> 2-bytes */

    for(auto iter : header.eventTimeInSeconds)
        readFromFile(&iter, SIZE_32_BIT, file); /* Read eventTimeInSeconds[i] -> 72-bytes */

    for(auto iter : header.eventDisplayFlags)
        readFromFile(&iter, SIZE_16_BIT, file); /* Read eventDisplayFlags[i] -> 18-bytes */

    readFromFile(&header.futureUseBlock_3, SIZE_16_BIT, file); /* Read futureUSeBlock_3 -> 2-bytes */

    for(auto iter : header.eventLabels)
        for(auto siter : iter)
            readFromFile(&siter, SIZE_8_BIT, file); /* Read eventLabels[i] -> 72-bytes */

    for(auto iter : header.futureUseBlock_4)
        readFromFile(&iter, SIZE_16_BIT, file); /* Read futureUSeBlock_4[i] -> 44-bytes */

    return header;
}

/* Swap the Endian Reading System */
Header_c3d Header_c3d::SwapHeaderEndianSystem(Header_c3d header) {
    Header_c3d headerBuf = header;

    headerBuf.parameterBlock = swapEndian(headerBuf.parameterBlock);
    headerBuf.idNumber = swapEndian(headerBuf.idNumber);
    headerBuf.pointsNumber = swapEndian(headerBuf.pointsNumber);
    headerBuf.analogNumber = swapEndian(headerBuf.analogNumber);
    headerBuf.firstFrame = swapEndian(headerBuf.firstFrame);
    headerBuf.lastFrame = swapEndian(headerBuf.lastFrame);
    headerBuf.maximumInterpolationGap = swapEndian(headerBuf.maximumInterpolationGap);
    headerBuf.scaleFactor = swapEndian(headerBuf.scaleFactor);
    headerBuf.dataStart = swapEndian(headerBuf.dataStart);
    headerBuf.analogPerFrame = swapEndian(headerBuf.analogPerFrame);
    headerBuf.frameRate = swapEndian(headerBuf.frameRate);

    for(auto &iter : headerBuf.futureUseBlock_1)
        iter = swapEndian(iter);

    headerBuf.keyValue_1 = swapEndian(headerBuf.keyValue_1);
    headerBuf.firstLabelRange = swapEndian(headerBuf.firstLabelRange);
    headerBuf.keyValue_2 = swapEndian(headerBuf.keyValue_2);
    headerBuf.eventTimeSize = swapEndian(headerBuf.eventTimeSize);
    headerBuf.futureUseBlock_2 = swapEndian(headerBuf.futureUseBlock_2);

    for(auto &iter : headerBuf.eventTimeInSeconds)
       iter = swapEndian(iter);

    for(auto &iter : headerBuf.eventDisplayFlags)
        iter = swapEndian(iter);

    headerBuf.futureUseBlock_3 = swapEndian(headerBuf.futureUseBlock_3);

    for(auto &iter : headerBuf.futureUseBlock_4)
        iter = swapEndian(iter);

    return headerBuf;
}

/* Print Header to File */
bool Header_c3d::PrintToFile(std::string path) {
    FILE* outputFile;
    outputFile = fopen(path.c_str(), "w");

    fprintf(outputFile, "parameter_block = %d\n", parameterBlock);
    fprintf(outputFile, "ADTech_ID_number = %d\n", idNumber);
    fprintf(outputFile, "Number_of_Points = %d\n", pointsNumber);
    fprintf(outputFile, "Number_of_Analog_data = %d\n", analogNumber);
    fprintf(outputFile, "First_Frame = %d\n", firstFrame);
    fprintf(outputFile, "Last_Frame = %d\n", lastFrame);
    fprintf(outputFile, "Max_Interpolation_Gap = %d\n", maximumInterpolationGap);
    fprintf(outputFile, "Point_Scale_Factor = %.4f\n", scaleFactor);
    fprintf(outputFile, "Data_Start_Block = %d\n", dataStart);
    fprintf(outputFile, "Analog_Per_Frame = %d\n", analogPerFrame);
    fprintf(outputFile, "Frame_Rate = %.4f Hz\n", frameRate);

    for(int i=0; i < futureBlock_1_size; ++i)
        fprintf(outputFile, "Future_use_block_[%d] = %d\n", i+1, futureUseBlock_1[i]);

    fprintf(outputFile, "Key_Value_1 = %d\n", keyValue_1);
    fprintf(outputFile, "First_label_Range_Block = %d\n", firstLabelRange);
    fprintf(outputFile, "Key_Value_2 = %d\n", keyValue_2);
    fprintf(outputFile, "Event_Time_Num = %d\n", eventTimeSize);
    fprintf(outputFile, "Future_use_block = %d\n", futureUseBlock_2);

    for(int i=0; i < eventTime_size; ++i)
        fprintf(outputFile, "Event_Time = %f sec\n", eventTimeInSeconds[i]);

    for(int i=0; i < displayFlag_size; ++i)
        fprintf(outputFile, "Event_Displat_Flag = %d\n", eventDisplayFlags[i]);

    fprintf(outputFile, "Future_use_block = %d\n", futureUseBlock_3);

    for(int i=0; i < eventLabel_size; ++i)
        fprintf(outputFile, "Event_Label_[%d] = %s\n", i+1, eventLabels[i].c_str());

    for(int i=0; i < futureBlock_4_size; ++i)
        fprintf(outputFile, "Future_use_block_[%d] = %d\n", i+1, futureUseBlock_4[i]);

    fclose(outputFile);

    return true;
}

