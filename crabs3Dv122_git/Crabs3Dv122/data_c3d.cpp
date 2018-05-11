#include "data_c3d.h"
#include "templates.h"

void Frame_c3d::ReadFrame(Frame_c3d& frame, FILE* file, const float pSize, const float aSize,
                          const float pointScale, const int flag) {
    frame.pointSize = pSize;
    frame.analogSize = aSize;

    frame.point = (Point_c3d*)malloc(frame.pointSize*sizeof(Point_c3d));
    frame.analog = (Analog_c3d*)malloc(frame.analogSize*sizeof(Analog_c3d));

    for(int i = 0; i < frame.pointSize; i++) {
        if(pointScale < 0) {
            float buf_x = 0.0;
            float buf_y = 0.0;
            float buf_z = 0.0;
            float buf_word_4_f = 0.0;
            short int buf_cam = 0.0;
            short int buf_res = 0.0;

            readFromFileWithEndian(&buf_x, SIZE_32_BIT, file, flag);
            readFromFileWithEndian(&buf_y, SIZE_32_BIT, file, flag);
            readFromFileWithEndian(&buf_z, SIZE_32_BIT, file, flag);
            readFromFileWithEndian(&buf_word_4_f, SIZE_32_BIT, file, flag);

            buf_cam = returnByte((short int)buf_word_4_f, 1);
            buf_res = returnByte((short int)buf_word_4_f, 2);

            frame.point[i] = frame.point->SetPoint(buf_x, buf_y, buf_z, (float) buf_cam, buf_res*(-pointScale));
        } else {
            short int buf_x = 0;
            short int buf_y = 0;
            short int buf_z = 0;
            short int buf_cam = 0;
            short int buf_res = 0;

            readFromFileWithEndian(&buf_x, SIZE_16_BIT, file, flag);
            readFromFileWithEndian(&buf_y, SIZE_16_BIT, file, flag);
            readFromFileWithEndian(&buf_z, SIZE_16_BIT, file, flag);
            readFromFileWithEndian(&buf_cam, SIZE_16_BIT, file, flag);
            readFromFileWithEndian(&buf_res, SIZE_16_BIT, file, flag);

            frame.point[i] = frame.point->SetPoint(buf_x*pointScale, buf_y*pointScale, buf_z*pointScale, (float) buf_cam, buf_res*pointScale);
        }
    }

    for(int i = 0; i < frame.analogSize; i++) {
        if(pointScale < 0) {
            float buf_analog = 0.0;

            readFromFileWithEndian(&buf_analog, SIZE_32_BIT, file, flag);

            frame.analog[i] = frame.analog->SetAnalog(buf_analog);
        } else {
            short int buf_analog = 0;

            readFromFileWithEndian(&buf_analog, SIZE_32_BIT, file, flag);

            frame.analog[i] = frame.analog->SetAnalog(buf_analog/pointScale);
        }
    }

}

/**********************************************************/

Data_c3d::Data_c3d()
{

}

Data_c3d Data_c3d::ReadData(Data_c3d data, FILE* file, const float scale, const int flag) {
    Data_c3d dataBuf = data;
    dataBuf.frame = (Frame_c3d*)malloc(dataBuf.frameSize*sizeof(Frame_c3d));

    for(int i = 0; i < dataBuf.frameSize; i++)
        dataBuf.frame->ReadFrame(dataBuf.frame[i], file, dataBuf.pointSize, dataBuf.analogDataSize, scale, flag);

    return dataBuf;
}

/* Print Points to CSV File */
bool Data_c3d::PrintPointsToFile_CSV(std::string path) {
    FILE* outputFile;

    outputFile = fopen(path.c_str(), "w");

    fprintf(outputFile, "FrameNumber");
    for(int i = 0; i < pointSize; i++) {
        fprintf(outputFile, ";X_%d;Y_%d;Z_%d;Camera_%d;Residual_%d", i+1, i+1, i+1, i+1, i+1);
    }
    fprintf(outputFile, "\n");
    for(int i = 0; i < frameSize; i++) {
        fprintf(outputFile, "%d", i+1);
        for(int j = 0; j < pointSize; j++) {
            fprintf(outputFile, ";%f;%f;%f;%f;%f", frame[i].Point(j).X(), frame[i].Point(j).Y(), frame[i].Point(j).Z(), frame[i].Point(j).Camera(), frame[i].Point(j).Residual());
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    return true;
}

/* Print Analog Data to CSV File */
bool Data_c3d::PrintAnalogToFile_CSV(std::string path) {
    FILE* outputFile;

    outputFile = fopen(path.c_str(), "w");

    fprintf(outputFile, "FrameNumber");
    for(int i = 0; i < analogDataSize; i++) {
        fprintf(outputFile, ";Analog_%d", i+1);
    }
    fprintf(outputFile, "\n");
    for(int i = 0; i < frameSize; i++) {
        fprintf(outputFile, "%d", i+1);
        for(int j = 0; j < analogDataSize; j++) {
            fprintf(outputFile, ";%f", frame[i].Analog(j).AnalogData());
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    return true;
}

