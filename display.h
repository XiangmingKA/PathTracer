#pragma once
#include <stdio.h>
#include <iostream>
//#include <eigen3/Eigen/Eigen>
#include <vector>
//using namespace Eigen;

extern class newFrameBuffer {

public:
    int width, height;
    std::vector <GColor> frame_buf;
    std::vector <float> depth_buf;


    newFrameBuffer(int w, int h) :width(w), height(h)
    {
        frame_buf.resize(w * h);
        depth_buf.resize(w * h);

        for (int i = 0; i<w*h; i++) {
            this->depth_buf[i] = 100;

        }
    }

    void set_pixel(const GVector3& point, const GColor& color, float depth)
    {
        //old index: auto ind = point.y() + point.x() * width;
        //auto ind = (height-point.y()-1)*width + point.x();
        auto ind = (height*(1 - point.y)) / 2 * width + (point.x + 1) / 2 * width;
        if (ind >= width*height || ind<0) {
            std::cout << "Set Pixel out of range: " << point.x << ", " << point.y << "\n";
        }
        else if (depth_buf[ind] >depth + 0.001)
        {
            frame_buf[ind] = color;
            depth_buf[ind] = depth;
            //std::cout<<(height*(1 - point.y())) / 2<<" , "<<(point.x() + 1) / 2 * width<<std::endl;

        }
    }


    void set_pixel(int x, int y, const GColor& color, float depth)
    {
        //old index: auto ind = point.y() + point.x() * width;
        //auto ind = (height-point.y()-1)*width + point.x();

        int ind = x + y*this->width;
        if (ind >= width*height || ind<0) {
            std::cout << "Set Pixel out of range: " << x << ", " << y << "\n";
        }
        else if (depth_buf[ind] >depth + 0.001) {
            frame_buf[ind] = color;
            depth_buf[ind] = depth;

        }


    }

    GColor getPixel(int i){
        this->frame_buf[i].r = this->frame_buf[i].r < 255? this->frame_buf[i].r : 255;
        this->frame_buf[i].g = this->frame_buf[i].g < 255? this->frame_buf[i].g: 255;
        this->frame_buf[i].b = this->frame_buf[i].b < 255? this->frame_buf[i].b : 255;

        GColor res (this->frame_buf[i].r,this->frame_buf[i].g,this->frame_buf[i].b);
        return res;
    }

};

extern newFrameBuffer myBuffer;
