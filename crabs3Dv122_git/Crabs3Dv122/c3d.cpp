#include "c3d.h"
#include "templates.h"

C3D::C3D() {
    isOpen = false;
    sysEndian = BIG_ENDIAN_PROCESSOR;
    fileEndian = LITTLE_ENDIAN_PROCESSOR;
}

bool C3D::Read_C3D_File(std::string filePath, C3D &c3d_f) {
    FILE* file;
    file = fopen(filePath.c_str(), "r"); // Open C3D file

    c3d_f.header = c3d_f.header.ReadHeaderBlock(file); // Read C3D Header Block
    c3d_f.parameter = c3d_f.parameter.ReadParameterHeaderBlock(file); // Read C3D Parameter Block
    if(system_endian::little_endial())
        sysEndian = LITTLE_ENDIAN_PROCESSOR;
    if(c3d_f.parameter.Header().Processor() == PROCESSOR_MIPS)
        fileEndian = BIG_ENDIAN_PROCESSOR;

    int endianFlag = SAME_ENDIAN;
    if(sysEndian != fileEndian) {
        endianFlag = DIFF_ENDIAN;
        c3d_f.header = c3d_f.header.SwapHeaderEndianSystem(c3d_f.header);
    }

    c3d_f.parameter = c3d_f.parameter.ReadGroupParameterBlock(file, c3d_f.parameter, endianFlag);

    int pSize = c3d_f.header.NumberOfPoints();
    int aSize = c3d_f.header.NumberOfAnalog();
    int fSize = c3d_f.header.LastFrame() - c3d_f.header.FirstFrame() + 1;

    c3d_f.data.SetSizes(c3d_f.data, fSize, pSize, aSize);
    c3d_f.data = c3d_f.data.ReadData(c3d_f.data, file, c3d_f.header.ScaleFactor(), endianFlag);

    c3d_f.trial = c3d_f.trial.SetTrial(c3d_f.parameter);
    c3d_f.subjects = c3d_f.subjects.SetSubjects(c3d_f.parameter);
    c3d_f.point = c3d_f.point.SetPoint(c3d_f.parameter);
    c3d_f.manufacturer = c3d_f.manufacturer.SetManufacturer(c3d_f.parameter);

    multiplier = c3d_f.point.MultiplierForMeters(c3d_f.point);

    c3d_f.cloud.SetCloud(c3d_f.cloud, c3d_f.data, c3d_f.point, multiplier);

   //c3d_f.cloud.PrintCloud();
   //c3d_f.cloud.PrintDist();

    return true;
}
