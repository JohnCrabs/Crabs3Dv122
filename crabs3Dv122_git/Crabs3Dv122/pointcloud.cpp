#include "pointcloud.h"
#include <cstdio>
#include <cmath>

#include <random>
#include <cmath>

#define CLUSTER_SIZE 16

auto absolute(auto num) {return (num < 0) ? -num : num;}

void Distance::SetDistance(Distance& d, my_float_ dx_, my_float_ dy_, my_float_ dz_, my_float_ id_start, my_float_ id_end) {
    d.dx = dx_;
    d.dy = dy_;
    d.dz = dz_;
    d.dist = sqrt(dx_*dx_ + dy_*dy_ + dz_*dz_);
    d.id_start_point = id_start;
    d.id_end_point = id_end;
}

//---------------------------------------------------------------------------------------------------------------------------------


void Frame::SetFrame(Frame &frame, Data_c3d data, Point point, const int frameIndex, const float multiplier) {

    frame.distState = false;
    frame.point = (ViewingPoint*)malloc(data.PointSize()*sizeof(ViewingPoint));

    frame.cluster = (Cluster*)malloc(CLUSTER_SIZE*sizeof(Cluster));

    float sign_x = 1.0;
    float sign_y = 1.0;

    if(point.X_Screen() != "" && point.Y_Screen() != "") {
        if(point.X_Screen() == "-X" || point.X_Screen() == "-Y" || point.X_Screen() == "-Z") {
            sign_x = -1.0;
        }

        if(point.Y_Screen() == "-X" || point.Y_Screen() == "-Y" || point.Y_Screen() == "-Z") {
            sign_y = -1.0;
        }

        if(point.X_Screen() == "-X" || point.X_Screen() == "+X") {
            if(point.Y_Screen() == "-Y" || point.Y_Screen() == "+Y") {
                for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).X()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).Y()*multiplier,
                                           -data.Frame(frameIndex).Point(j).Z()*multiplier);

            } else if(point.Y_Screen() == "-Z" || point.Y_Screen() == "+Z") {
                for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).X()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).Z()*multiplier,
                                           -data.Frame(frameIndex).Point(j).Y()*multiplier);
            }
        } else if(point.X_Screen() == "-Y" || point.X_Screen() == "+Y") {
            if(point.Y_Screen() == "-X" || point.Y_Screen() == "+X") {
                for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).Y()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).X()*multiplier,
                                           -data.Frame(frameIndex).Point(j).Z()*multiplier);

            } else if(point.Y_Screen() == "-Z" || point.Y_Screen() == "+Z") {
                for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).Y()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).Z()*multiplier,
                                           -data.Frame(frameIndex).Point(j).X()*multiplier);

            }
        } else if(point.X_Screen() == "-Z" || point.X_Screen() == "+Z") {
            if(point.Y_Screen() == "-X" || point.Y_Screen() == "+X") {for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).Z()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).X()*multiplier,
                                           -data.Frame(frameIndex).Point(j).Y()*multiplier);

            } else if(point.Y_Screen() == "-Y" || point.Y_Screen() == "+Y") {
                for(int j = 0; j < data.PointSize(); j++)
                    frame.point->SetPoint(frame.point[j], j, 0, sign_x*data.Frame(frameIndex).Point(j).Z()*multiplier,
                                          sign_y*data.Frame(frameIndex).Point(j).Y()*multiplier,
                                           -data.Frame(frameIndex).Point(j).X()*multiplier);

            }
        }
    }

    for(int i = 0; i != data.PointSize(); ++i) {
        frame.point->Set_Name(frame.point[i], point.Labels(i));
    }

    frame.distSize = data.PointSize() * data.PointSize();
    frame.dist = (Distance*)malloc(frame.distSize * sizeof(Distance));
    for(int i = 0; i != data.PointSize(); ++i)
        for(int j = 0; j != data.PointSize(); ++j) {
            frame.dist->SetDistance(frame.dist[i*data.PointSize() + j], frame.point[i].X() - frame.point[j].X(),
                                    frame.point[i].Y() - frame.point[j].Y(), frame.point[i].Z() - frame.point[j].Z(),
                                    frame.point[i].ID(), frame.point[j].ID());
        }



}


int Frame::findMin_X(Frame &frame, int pointSize) {
    int index;
    my_float_ min = frame.Point_(0).X();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).X() < min && frame.Point_(i).Visible()) {
            min = frame.Point_(i).X();
            index = i;
        }
    }

    return index;
}

int Frame::findMin_Y(Frame &frame, int pointSize) {
    int index;
    my_float_ min = frame.Point_(0).Y();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).Y() < min && frame.Point_(i).Visible()) {
            min = frame.Point_(i).Y();
            index = i;
        }
    }

    return index;
}

int Frame::findMin_Z(Frame &frame, int pointSize) {
    int index;
    my_float_ min = frame.Point_(0).Z();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).Z() < min && frame.Point_(i).Visible()) {
            min = frame.Point_(i).Z();
            index = i;
        }
    }

    return index;
}



int Frame::findMax_X(Frame &frame, int pointSize) {
    int index;
    my_float_ max = frame.Point_(0).X();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).X() > max && frame.Point_(i).Visible()) {
            max = frame.Point_(i).X();
            index = i;
        }
    }

    return index;
}

int Frame::findMax_Y(Frame &frame, int pointSize) {
    int index;
    my_float_ max = frame.Point_(0).Y();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).Y() > max && frame.Point_(i).Visible()) {
            max = frame.Point_(i).Y();
            index = i;
        }
    }

    return index;
}

int Frame::findMax_Z(Frame &frame, int pointSize) {
    int index;
    my_float_ max = frame.Point_(0).Z();

    for(int i = 1; i != pointSize; ++i) {
        if(frame.Point_(i).Z() > max && frame.Point_(i).Visible()) {
            max = frame.Point_(i).Z();
            index = i;
        }
    }

    return index;
}

//---------------------------------------------------------------------------------------------------------------------------------

PointCloud::PointCloud()
{

}

void PointCloud::SetCloud(PointCloud& cloud, Data_c3d data, Point point, const float multiplier) {
    cloud.frame = (Frame*)malloc(data.FrameSize()*sizeof(Frame));
    cloud.frameSize = data.FrameSize();
    cloud.pointSize = data.PointSize();
    for(int i = 0; i < data.FrameSize(); i++)
        cloud.frame->SetFrame(cloud.frame[i], data, point, i, multiplier);

    my_float_ c_x = 0.0;
    my_float_ c_y = 0.0;
    my_float_ c_z = 0.0;
    for(int i = 0; i != cloud.PointSize(); ++i) {
        c_x = cloud.frame[0].Point_(i).X();
        c_y = cloud.frame[0].Point_(i).Y();
        c_z = cloud.frame[0].Point_(i).Z();
    }


    cloud.clusterSize = 1;
    cloud.cluster = (Cluster*)malloc(CLUSTER_SIZE*sizeof(Cluster));
    cloud.cluster->SetColor(cloud.cluster[0], 255, 255, 0);
    cloud.cluster->SetId(cloud.cluster[0], 0);
    cloud.cluster->SetPointSize(cloud.cluster[0], 2.0);
    cloud.cluster->SetCentroid(cloud.cluster[0], c_x/cloud.pointSize, c_y/cloud.pointSize, c_z/cloud.pointSize);
    cloud.cluster->SetName(cloud.cluster[0], "Cluster_0");
    cloud.cluster->SetPointsInCluster(cloud.cluster[0], cloud.pointSize);
    cloud.cluster->SetVisibility(cloud.cluster[0], true);

    cloud.frame->SetCentroid(cloud.frame[0], 0, 0.0, 0.0, 0.0);
}

void PointCloud::PrintCloud(void) {

    FILE* o_file;
    o_file = fopen("cloud.csv", "w");

    for(int i = 0; i != frameSize; ++i) {
        for(int j = 0; j != pointSize; ++j) {
            fprintf(o_file, "%.5f,%.5f,%.5f", frame[i].Point_(j).X(), frame[i].Point_(j).Y(), frame[i].Point_(j).Z());
            if((pointSize - 1) != j)
                fprintf(o_file, ",");
        }
        fprintf(o_file, "\n");
    }

    fclose(o_file);
}

void PointCloud::PrintCloud(int fromFrame, int toFrame, char seperator, std::string path) {
    FILE* o_file;
    o_file = fopen(path.c_str(), "w");

    for(int i = fromFrame; i != toFrame; ++i) {
        for(int j = 0; j != pointSize; ++j) {
            if(frame[i].Point_(j).Visible()) {
                fprintf(o_file, "%.3f%c%.3f%c%.3f", frame[i].Point_(j).X(), seperator,
                                                    frame[i].Point_(j).Y(), seperator,
                                                    frame[i].Point_(j).Z());
                if((pointSize - 1) != j)
                    fprintf(o_file, "%c", seperator);
            }
        }
        fprintf(o_file, "\n");
    }

    fclose(o_file);
}

void PointCloud::PrintClusters(int fromFrame, int toFrame, char seperator, std::string path) {
    for(int c_index = 1; c_index != clusterSize; ++c_index) {
        FILE* o_file;
        std::string str_path = path;
        str_path += cluster[c_index].Name() + ".csv";
        o_file = fopen(str_path.c_str(), "w");

        for(int i = fromFrame; i != toFrame; ++i) {
            for(int j = 0; j != pointSize; ++j) {
                if(frame[i].Point_(j).Visible() && frame[i].Point_(j).C_ID() == c_index) {
                    fprintf(o_file, "%.3f%c%.3f%c%.3f", frame[i].Point_(j).X(), seperator,
                                                        frame[i].Point_(j).Y(), seperator,
                                                        frame[i].Point_(j).Z());
                    if((pointSize - 1) != j)
                        fprintf(o_file, "%c", seperator);
                }
            }
            fprintf(o_file, "\n");
        }

        fclose(o_file);
    }

    FILE* o_file;
    std::string str_path = path;
    str_path += "centroids.csv";
    o_file = fopen(str_path.c_str(), "w");
    for(int i = 1; i < clusterSize; ++i) {
        fprintf(o_file, "%s%c%c%c", cluster[i].Name().c_str(), seperator, seperator,seperator);
    }
    fprintf(o_file, "\n");
    for(int f_index = fromFrame; f_index != toFrame; ++f_index) {
        for(int c_index = 1; c_index != clusterSize; ++c_index) {
            fprintf(o_file, "%.3f%c%.3f%c%.3f%c", frame[f_index].Cluster_(c_index).Centroid_().X(), seperator,
                                                frame[f_index].Cluster_(c_index).Centroid_().Y(), seperator,
                                                frame[f_index].Cluster_(c_index).Centroid_().Z(), seperator);

        }
        fprintf(o_file, "\n");
    }

    fclose(o_file);
}

void PointCloud::PrintDist(void) {
    FILE* o_file;
    o_file = fopen("dist.csv", "w");

    for(int fr = 0; fr != frameSize; ++fr) {
        for(int i = 0; i != frame[fr].DistSize_(); ++i) {
            if(i%pointSize == 0)
                fprintf(o_file, "\n");
            fprintf(o_file, "%d_%d;%.3f;%.3f;%.3f;%.3f;;", frame[fr].Dist_(i).ID_START_(), frame[fr].Dist_(i).ID_END_(),
                                                           frame[fr].Dist_(i).DX_(), frame[fr].Dist_(i).DY_(),
                                                           frame[fr].Dist_(i).DZ_(), frame[fr].Dist_(i).DIST_());
        }
        fprintf(o_file, "\n\n");
    }

    fclose(o_file);
}

void PointCloud::AccumulatedNoiseRemoval(PointCloud& cloud, my_float_ thresh, my_float_ tolerance) {
    int cnt;
    my_float_ tol = tolerance / 100;
    my_float_ Dx;
    my_float_ Dy;
    my_float_ Dz;
    my_float_ Ddist;
    for(int i = 0; i != cloud.PointSize(); ++i) {
        cnt = 0;
        for(int j = 0; j != cloud.FrameSize(); ++j) {
            if(cloud.Frame_(j).Point_(i).Visible()) {
                Dx = cloud.Frame_(0).Point_(i).X() - cloud.Frame_(j).Point_(i).X();
                Dy = cloud.Frame_(0).Point_(i).Y() - cloud.Frame_(j).Point_(i).Y();
                Dz = cloud.Frame_(0).Point_(i).Z() - cloud.Frame_(j).Point_(i).Z();
                Ddist = sqrt(Dx*Dx + Dy*Dy + Dz*Dz);
                if(Ddist < thresh)
                    ++cnt;
            }
        }
        if(cnt > cloud.FrameSize()*tol)
            for(int j = 0; j != cloud.FrameSize(); ++j)
                cloud.frame->SetPointVisibility(cloud.frame[j], i, false);
    }
}

void PointCloud::NearestNeighborRemoval(PointCloud& cloud, my_float_ thresh, int tolerance) {
    int cnt;
    int id_start;

    for(int fr = 0; fr != cloud.FrameSize(); ++fr) {
        cnt = 0;
        for(int i = 0; i != cloud.Frame_(fr).DistSize_(); ++i) {
            if(cloud.Frame_(fr).Dist_(i).DIST_() < thresh)
                ++cnt;
            if(i%cloud.PointSize() == 0) {
                if(cnt-1 < tolerance) {
                    id_start = cloud.Frame_(fr).Dist_(i).ID_START_();
                    for(int j = 0; j != cloud.FrameSize(); ++j)
                        cloud.frame->SetPointVisibility(cloud.frame[j], id_start, false);
                }
                cnt = 0;
            }
        }
    }

}

void PointCloud::FixCloudLinear(PointCloud& cloud) {
    my_float_ *DFrame_x;
    my_float_ *DFrame_y;
    my_float_ *DFrame_z;

    DFrame_x = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );
    DFrame_y = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );
    DFrame_z = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );


    my_float_ tolerance_x;
    my_float_ tolerance_y;
    my_float_ tolerance_z;

    my_float_ dstep_x;
    my_float_ dstep_y;
    my_float_ dstep_z;

    for(int p_index = 0; p_index != cloud.pointSize; ++p_index) {

        tolerance_x = 0.0;
        tolerance_y = 0.0;
        tolerance_z = 0.0;

        for(int f_index = 1; f_index != cloud.frameSize; ++f_index) {
            DFrame_x[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).X() - cloud.Frame_(f_index - 1).Point_(p_index).X();
            DFrame_y[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).Y() - cloud.Frame_(f_index - 1).Point_(p_index).Y();
            DFrame_z[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).Z() - cloud.Frame_(f_index - 1).Point_(p_index).Z();

            tolerance_x += absolute(DFrame_x[f_index - 1]);
            tolerance_y += absolute(DFrame_y[f_index - 1]);
            tolerance_z += absolute(DFrame_z[f_index - 1]);
        }

        tolerance_x /= cloud.frameSize - 1;
        tolerance_y /= cloud.frameSize - 1;
        tolerance_z /= cloud.frameSize - 1;

        dstep_x = tolerance_x;
        dstep_y = tolerance_y;
        dstep_z = tolerance_z;

        tolerance_x *= 10;
        tolerance_y *= 10;
        tolerance_z *= 10;

        for(int f_index = 1; f_index != cloud.frameSize - 1; ++f_index) {

            if( absolute(DFrame_x[f_index]) > tolerance_x) {

                dstep_x = DFrame_x[f_index-1];

                DFrame_x[f_index] = dstep_x;
                cloud.frame->Set_X(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).X() + dstep_x);

                if(f_index < cloud.frameSize - 2) {
                    DFrame_x[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).X() - cloud.Frame_(f_index).Point_(p_index).X();
                }
            }
            if( absolute(DFrame_y[f_index]) > tolerance_y) {

                dstep_y = DFrame_y[f_index-1];

                DFrame_y[f_index] = dstep_y;
                cloud.frame->Set_Y(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).Y() + dstep_y);

                if(f_index < cloud.frameSize - 2) {
                    DFrame_y[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).Y() - cloud.Frame_(f_index).Point_(p_index).Y();
                }
            }
            if( absolute(DFrame_z[f_index]) > tolerance_z) {

                dstep_z = DFrame_z[f_index-1];

                DFrame_z[f_index] = dstep_z;
                cloud.frame->Set_Z(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).Z() + dstep_z);
                if(f_index < cloud.frameSize - 2) {
                    DFrame_z[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).Z() - cloud.Frame_(f_index).Point_(p_index).Z();
                }
            }
        }

    }

    free(DFrame_x);
    free(DFrame_y);
    free(DFrame_z);

}

void PointCloud::FixCloudLinearWithWeights(PointCloud& cloud) {
    my_float_ *DFrame_x;
    my_float_ *DFrame_y;
    my_float_ *DFrame_z;

    DFrame_x = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );
    DFrame_y = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );
    DFrame_z = (my_float_*)malloc( ( cloud.frameSize - 1 ) * sizeof(my_float_) );


    my_float_ tolerance_x;
    my_float_ tolerance_y;
    my_float_ tolerance_z;

    my_float_ dstep_x;
    my_float_ dstep_y;
    my_float_ dstep_z;

    int weight_x;
    int weight_y;
    int weight_z;

    bool resetW_x = true;
    bool resetW_y = true;
    bool resetW_z = true;

    for(int p_index = 0; p_index != cloud.pointSize; ++p_index) {

        tolerance_x = 0.0;
        tolerance_y = 0.0;
        tolerance_z = 0.0;

        for(int f_index = 1; f_index != cloud.frameSize; ++f_index) {
            DFrame_x[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).X() - cloud.Frame_(f_index - 1).Point_(p_index).X();
            DFrame_y[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).Y() - cloud.Frame_(f_index - 1).Point_(p_index).Y();
            DFrame_z[f_index - 1] = cloud.Frame_(f_index).Point_(p_index).Z() - cloud.Frame_(f_index - 1).Point_(p_index).Z();

            tolerance_x += absolute(DFrame_x[f_index - 1]);
            tolerance_y += absolute(DFrame_y[f_index - 1]);
            tolerance_z += absolute(DFrame_z[f_index - 1]);
        }

        tolerance_x /= cloud.frameSize - 1;
        tolerance_y /= cloud.frameSize - 1;
        tolerance_z /= cloud.frameSize - 1;

        dstep_x = tolerance_x;
        dstep_y = tolerance_y;
        dstep_z = tolerance_z;

        tolerance_x *= 10;
        tolerance_y *= 10;
        tolerance_z *= 10;

        for(int f_index = 1; f_index != cloud.frameSize - 1; ++f_index) {

            if(resetW_x) {
                weight_x = 1;
                resetW_x = false;
            }
            if(resetW_y) {
                weight_y = 1;
                resetW_y = false;
            }
            if(resetW_z) {
                weight_z = 1;
                resetW_z = false;
            }

            if( absolute(DFrame_x[f_index]) > tolerance_x) {

                dstep_x = DFrame_x[f_index-1];

                DFrame_x[f_index] = dstep_x;
                cloud.frame->Set_X(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).X() + dstep_x/weight_x);
                ++weight_x;

                if(f_index < cloud.frameSize - 2) {
                    DFrame_x[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).X() - cloud.Frame_(f_index).Point_(p_index).X();
                    if( absolute(DFrame_x[f_index]) > tolerance_x) {
                        resetW_x = true;
                    }
                }
            }
            if( absolute(DFrame_y[f_index]) > tolerance_y) {

                dstep_y = DFrame_y[f_index-1]/weight_y;

                DFrame_y[f_index] = dstep_y;
                cloud.frame->Set_Y(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).Y() + dstep_y/weight_y);
                ++weight_y;

                if(f_index < cloud.frameSize - 2) {
                    DFrame_y[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).Y() - cloud.Frame_(f_index).Point_(p_index).Y();
                    if( absolute(DFrame_x[f_index]) > tolerance_y) {
                        resetW_y = true;
                    }
                }
            }
            if( absolute(DFrame_z[f_index]) > tolerance_z) {

                dstep_z = DFrame_z[f_index-1]/weight_z;

                DFrame_z[f_index] = dstep_z;
                cloud.frame->Set_Z(cloud.frame[f_index], p_index, cloud.Frame_(f_index - 1).Point_(p_index).Z() + dstep_z/weight_z);
                ++weight_z;

                if(f_index < cloud.frameSize - 2) {
                    DFrame_z[f_index + 1] = cloud.Frame_(f_index + 1).Point_(p_index).Z() - cloud.Frame_(f_index).Point_(p_index).Z();
                    if( absolute(DFrame_x[f_index]) > tolerance_z) {
                        resetW_z = true;
                    }
                }
            }
        }

    }

    free(DFrame_x);
    free(DFrame_y);
    free(DFrame_z);

}

void PointCloud::K_means(PointCloud& cloud, int frameIndex, int clusterSize) {


    //If clusterSize = 1, then put all points in Cluster 0
    if(clusterSize == 1) {
        cloud.clusterSize = clusterSize;
        for(int p_size = 0; p_size != cloud.PointSize(); ++p_size) {
            for(int f_size = 0; f_size != cloud.FrameSize(); ++f_size) {
                cloud.frame->Set_CID(cloud.frame[f_size], p_size, 0);
                cloud.cluster->SetVisibility(cloud.cluster[0], true);
            }
            cloud.cluster->SetPointsInCluster(cloud.cluster[0], cloud.PointSize());
        }

    } else { //Calculate Clusters

        //Create K_Means Parameters and Allocate Memory
        cloud.clusterSize = clusterSize + 1;
        my_float_ dx;
        my_float_ dy;
        my_float_ dz;
        bool loop = true;

        my_float_ *dist;
        dist = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ *c_x_new;
        c_x_new = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ *c_y_new;
        c_y_new = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ *c_z_new;
        c_z_new = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        int *c_size;
        c_size = (int*)malloc((clusterSize+1)*sizeof(int));

        int *p_index_size;
        p_index_size = (int*)malloc((clusterSize+1)*sizeof(int));

        my_float_ *c_x_old;
        c_x_old = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ *c_y_old;
        c_y_old = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ *c_z_old;
        c_z_old = (my_float_*)malloc((clusterSize+1)*sizeof(my_float_));

        my_float_ mindist;
        int mindistIndex;

        std::string c_name = "Cluster_";

        int buf_index;

        for(int i = 1; i <= clusterSize; ++i) {
            cloud.frame->Set_CID(cloud.frame[frameIndex], i-1, i);

            cloud.cluster->SetColor(cloud.cluster[i], (rand() % 255), (rand() % 255), (rand() % 255));
            cloud.cluster->SetPointSize(cloud.cluster[i], 2.0);
            cloud.cluster->SetId(cloud.cluster[i], i);
            cloud.cluster->SetName(cloud.cluster[i], c_name + std::to_string(i));
            cloud.cluster->SetVisibility(cloud.cluster[i], true);

            if(i == 1) {
                buf_index = cloud.frame->findMin_X(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 2) {
                buf_index = cloud.frame->findMax_X(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 3) {
                buf_index = cloud.frame->findMin_Y(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 4) {
                buf_index = cloud.frame->findMax_Y(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 5) {
                buf_index = cloud.frame->findMin_Z(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 6) {
                buf_index = cloud.frame->findMax_Z(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).X();
                c_y_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Y();
                c_z_old[i] = cloud.Frame_(frameIndex).Point_(buf_index).Z();
            } else if(i == 7) {
                c_x_old[i] = c_x_old[1] + (c_x_old[2] - c_x_old[1]) / 2;
                c_y_old[i] = c_y_old[1] + (c_y_old[2] - c_y_old[1]) / 2;
                c_z_old[i] = c_z_old[1] + (c_z_old[2] - c_z_old[1]) / 2;
            } else if(i == 8) {
                buf_index = cloud.frame->findMax_Z(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = c_x_old[3] + (c_x_old[4] - c_x_old[3]) / 2;
                c_y_old[i] = c_y_old[3] + (c_y_old[4] - c_y_old[3]) / 2;
                c_z_old[i] = c_z_old[3] + (c_z_old[4] - c_z_old[3]) / 2;
            } else if(i == 9) {
                buf_index = cloud.frame->findMax_Z(cloud.frame[frameIndex], cloud.PointSize());
                c_x_old[i] = c_x_old[5] + (c_x_old[6] - c_x_old[5]) / 2;
                c_y_old[i] = c_y_old[5] + (c_y_old[6] - c_y_old[5]) / 2;
                c_z_old[i] = c_z_old[5] + (c_z_old[6] - c_z_old[5]) / 2;
            }

            else { //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX we need better code

                //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
            }
        }

        while (loop) {
            for(int p_index = 0; p_index != cloud.PointSize(); ++p_index) {
                if(cloud.Frame_(frameIndex).Point_(p_index).Visible()) {
                    for(int i = 1; i <= clusterSize; ++i) {

                        c_x_new[i] = 0.0;
                        c_y_new[i] = 0.0;
                        c_z_new[i] = 0.0;
                        c_size[i] = 0;

                        dx = c_x_old[i] - cloud.Frame_(frameIndex).Point_(p_index).X();
                        dy = c_y_old[i] - cloud.Frame_(frameIndex).Point_(p_index).Y();
                        dz = c_z_old[i] - cloud.Frame_(frameIndex).Point_(p_index).Z();

                        dist[i] = absolute(dx) + absolute(dy) + absolute(dz);
                    }

                    mindist = dist[1];
                    mindistIndex = 1;

                    for(int j = 2; j <= clusterSize; ++j) {
                        if(dist[j] < mindist) {
                            mindist = dist[j];
                            mindistIndex = j;
                        }
                    }

                    cloud.frame->Set_CID(cloud.frame[frameIndex], p_index, mindistIndex);

                    c_x_new[mindistIndex] += cloud.Frame_(frameIndex).Point_(p_index).X();
                    c_y_new[mindistIndex] += cloud.Frame_(frameIndex).Point_(p_index).Y();
                    c_z_new[mindistIndex] += cloud.Frame_(frameIndex).Point_(p_index).Z();
                    ++c_size[mindistIndex];
                }
            }

            for(int i = 1; i <= clusterSize; ++i) {
                c_x_new[i] /= c_size[i];
                c_y_new[i] /= c_size[i];
                c_z_new[i] /= c_size[i];
            }

            loop = false;
            for(int i = 1; i <= clusterSize; ++i) {

                dx = c_x_old[i] - c_x_new[i];
                dy = c_y_old[i] - c_y_new[i];
                dz = c_z_old[i] - c_z_new[i];

                if(absolute(dx) > 0.0001 || absolute(dy) > 0.0001 || absolute(dz) > 0.0001) {

                    c_x_old[i] = c_x_new[i];
                    c_y_old[i] = c_y_new[i];
                    c_z_old[i] = c_z_new[i];

                    loop = true;
                }
            }

        } //end loop (while)

        for(int f_index = 0; f_index != cloud.FrameSize(); ++f_index) {

            for(int i = 1; i <= clusterSize; ++i) {
                c_x_new[i] = 0.0;
                c_y_new[i] = 0.0;
                c_z_new[i] = 0.0;
                c_size[i] = 0;
            }

            for(int p_index = 0; p_index != cloud.PointSize(); ++p_index) {
                if(cloud.Frame_(f_index).Point_(p_index).Visible()) {
                    mindistIndex = cloud.Frame_(frameIndex).Point_(p_index).C_ID();

                    c_x_new[mindistIndex] += cloud.Frame_(f_index).Point_(p_index).X();
                    c_y_new[mindistIndex] += cloud.Frame_(f_index).Point_(p_index).Y();
                    c_z_new[mindistIndex] += cloud.Frame_(f_index).Point_(p_index).Z();
                    ++c_size[mindistIndex];

                    cloud.frame->Set_CID(cloud.frame[f_index], p_index, mindistIndex);
                }
            }

            for(int i = 1; i <= clusterSize; ++i) {
                c_x_new[i] /= c_size[i];
                c_y_new[i] /= c_size[i];
                c_z_new[i] /= c_size[i];

                cloud.frame->SetCentroid(cloud.frame[f_index], i, c_x_new[i], c_y_new[i], c_z_new[i]);
            }

        }



        for(int i = 0; i <= clusterSize; ++i) {
            p_index_size[i] = 0;
            cloud.cluster->SetPointsInCluster(cloud.cluster[i], 0);
            for(int p_index = 0; p_index != cloud.PointSize(); ++p_index) {
                if(i == cloud.Frame_(frameIndex).Point_(p_index).C_ID())
                    cloud.cluster->SetPointsInCluster(cloud.cluster[i], ++p_index_size[i]);
            }
        }

        free(dist);
        free(c_x_new);
        free(c_y_new);
        free(c_z_new);
        free(c_x_old);
        free(c_y_old);
        free(c_z_old);
        free(c_size);
        free(p_index_size);
    }
}
