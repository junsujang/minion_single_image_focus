/* -----------------------------------------------------------------
 * File:    Image.cpp
 * Created: 2015-08-29
 * -----------------------------------------------------------------
 *
 * The 6.815/6.865 Image class
 *
 * ---------------------------------------------------------------*/


#include "Image.h"


using namespace std;

// --------- HANDOUT  PS00 ------------------------------
// ------------------------------------------------------

long long Image::number_of_elements()const {
    // --------- HANDOUT  PS00 ------------------------------
    // returns the number of elements in the im- age. An RGB (3 color channels)
    // image of 100 × 100 pixels has 30000 elements
    if (dimensions() == 1) {
        return width();
    } else if (dimensions() == 2) {
        return width() * height();
    } else {
        return width() * height() * channels();
    }
}


// -------------- Accessors and Setters -------------------------
const float & Image::operator()(int x) const {
    // --------- HANDOUT  PS00 ------------------------------
    // Linear accessor to the image data
    if (x < 0 || x > number_of_elements()) {
        throw OutOfBoundsException();
    }
    return image_data[x];
}


const float & Image::operator()(int x, int y) const {
    // --------- HANDOUT  PS00 ------------------------------
    // Accessor to the image data at channel 0
    if ((x < 0 || x > width()) || (y < 0 || y > height())) {
        throw OutOfBoundsException();
    }
    return image_data[y*stride(1)+x];
}


const float & Image::operator()(int x, int y, int z) const {
    // --------- HANDOUT  PS00 ------------------------------
    // Accessor to the image data at channel z
    if ((x < 0 || x > width()) 
        || (y < 0 || y > height()) 
        || (z < 0 || z > channels())) {
        throw OutOfBoundsException();
    }
    return image_data[z*stride(2)+y*stride(1)+x];
}


float & Image::operator()(int x) {
    // --------- HANDOUT  PS00 ------------------------------
    // Linear setter to the image data
    if (x < 0 || x > number_of_elements()) {
        throw OutOfBoundsException();
    }
    return image_data[x];
}


float & Image::operator()(int x, int y) {
    // --------- HANDOUT  PS00 ------------------------------
    // Setter to the image data at channel 0
    if ((x < 0 || x > width()) || (y < 0 || y > height())) {
        throw OutOfBoundsException();
    }
    return image_data[y*stride(1)+x];
}


float & Image::operator()(int x, int y, int z) {
    // --------- HANDOUT  PS00 ------------------------------
    // Setter to the image data at channel z
    if ((x < 0 || x > width()) 
        || (y < 0 || y > height()) 
        || (z < 0 || z > channels())) {
        throw OutOfBoundsException();
    }
    return image_data[z*stride(2)+y*stride(1)+x];
}

void Image::set_color(float r, float g, float b) {
    // --------- HANDOUT  PS00 ------------------------------
    // Set the image pixels to the corresponding values
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
        throw OutOfBoundsException();
    }
    if (dimensions() < 3) {
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                image_data[i+j*stride(1)] = r;
            }
        }
    } else if (dimensions() == 3) {
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                image_data[i+j*stride(1)] = r;
                image_data[i+j*stride(1)+1*stride(2)] = g;
                image_data[i+j*stride(1)+2*stride(2)] = b;
            }
        }
    } else {
        throw OutOfBoundsException();
    }
}


void Image::create_rectangle(int xstart, int ystart, int xend, int yend,
                             float r, float g, float b) {
    // --------- HANDOUT  PS00 ------------------------------
    // Set the pixels inside the rectangle to the specified color
    if (xstart < 0 || ystart < 0 || xend >= width() || yend >= height()) {
        throw OutOfBoundsException();
    }
    if (xstart > xend || ystart > yend) {
        throw InvalidArgument();
    }
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
        throw InvalidArgument();
    }
    if (dimensions() < 3) {
        for (int j = ystart; j <= yend; j++) {
            for (int i = xstart; i <= xstart; i++) {
                image_data[i+j*stride(1)] = r;
            }
        }
    } else if (dimensions() == 3) {
        for (int j = ystart; j <= yend; j++) {
            for (int i = xstart; i <= xend; i++) {
                image_data[i+j*stride(1)] = r;
                image_data[i+j*stride(1)+1*stride(2)] = g;
                image_data[i+j*stride(1)+2*stride(2)] = b;
            }
        }
    } else {
        throw OutOfBoundsException();
    }
}

void Image::create_line(int xstart, int ystart, int xend, int yend,
                        float r, float g, float b) {
    // --------- HANDOUT  PS00 ------------------------------
    // Create a line segment with specified color
    if (xstart < 0 || ystart < 0 || xend >= width() || yend >= height()) {
        throw OutOfBoundsException();
    }
    if (xstart > xend || ystart > yend) {
        throw InvalidArgument();
    }
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
        throw InvalidArgument();
    }

    // Inspired by Bresenham's line algorithm
    float x_diff = xend - xstart;
    float y_diff = yend - ystart;
    int y_gradient = int((0.0f < y_diff) - (y_diff < 0.0f));
    float delta_err;
    if (xstart == xend) {
        delta_err = y_diff;
    } else {
        delta_err = abs(y_diff/x_diff);
    }
    float err = 0.0f;
    int y = ystart;
    for (int x = xstart; x <= xend; x++) {
        image_data[x + y*stride(1)] = r;
        if (dimensions() == 3) {
            image_data[x + y*stride(1) + 1*stride(2)] = g;
            image_data[x + y*stride(1) + 2*stride(2)] = b;
        }
        err += delta_err;
        while (err > 0.5f) {
            y = y + y_gradient;
            image_data[x + y*stride(1)] = r;
            if (dimensions() == 3) {
                image_data[x + y*stride(1) + 1*stride(2)] = g;
                image_data[x + y*stride(1) + 2*stride(2)] = b;
            }
            err -= 1.0f;
        }
    }
}

// ---------------- END of PS00 -------------------------------------


/*********************************************************************
 *                    DO NOT EDIT BELOW THIS LINE                    *
 *********************************************************************/

int Image::debugWriteNumber = 0;

Image::Image(int x, int y, int z, const std::string &name_) {
    initialize_image_metadata(x,y,z,name_);
    long long size_of_data = 1;
    for (int k = 0; k < dimensions(); k++) {
        size_of_data *= dim_values[k];
    }
    image_data = std::vector<float>(size_of_data,0);

}

void Image::initialize_image_metadata(int x, int y, int z,  const std::string &name_) {
    dim_values[0] = 0;
    dim_values[1] = 0;
    dim_values[2] = 0;
    stride_[0] = 0;
    stride_[1] = 0;
    stride_[2] = 0;

    dims = 0;
    long long size_of_data = 1;
    if ( x < 0 )
        throw NegativeDimensionException();
    if ( y< 0)
        throw NegativeDimensionException();
    if (z < 0 )
        throw NegativeDimensionException();

    image_name = name_;


    dims++;
    dim_values[0] = x;
    size_of_data *= x;
    stride_[0] = 1;
    if (y > 0 ) {
        dims++;
        dim_values[1] = y;
        size_of_data *= y;
        stride_[1] = x;
    } else {
        return;
    }

    if (z>0)  {
        dims++;
        dim_values[2] =z;
        size_of_data *= z;
        stride_[2] = x*y;
    } else {
        return;
    }

}

Image::Image(const std::string & filename) {
    std::vector<unsigned char> uint8_image;
    unsigned int height_;
    unsigned int width_;
    unsigned int channels_ = 4;
    unsigned int outputchannels_ = 3; // Throw away transparency
    unsigned err = lodepng::decode(uint8_image, width_, height_, filename.c_str()); // In column major order with packed color values
    if(err == 48) {
        throw FileNotFoundException();
    }

    image_data = std::vector<float>(height_*width_*outputchannels_,0);

    for (unsigned int x= 0; x < width_; x++) {
        for (unsigned int y = 0; y < height_; y++) {
            for (unsigned int c = 0; c < outputchannels_; c++) {
                image_data[x+y*width_+c*width_*height_] = uint8_to_float(uint8_image[c + x*channels_ + y*channels_*width_]);
            }
        }
    }

    initialize_image_metadata(width_, height_, outputchannels_, filename);

}

Image::~Image() { } // Nothing to clean up

void Image::write(const std::string &filename) const {
    if (channels() != 1 && channels() != 3 && channels() != 4)
        throw ChannelException();
    int png_channels = 4;
    std::vector<unsigned char> uint8_image(height()*width()*png_channels, 255);
    int c;
    for (int x= 0; x < width(); x++) {
        for (int y = 0; y < height(); y++) {
            for (c = 0; c < channels(); c++) {
                uint8_image[c + x*png_channels + y*png_channels*width()] = float_to_uint8(image_data[x+y*width()+c*width()*height()]);
            }
            for ( ; c < 3; c++) { // Only executes when there is one channel

                uint8_image[c + x*png_channels + y*png_channels*width()] = float_to_uint8(image_data[x+y*width()+0*width()*height()]);
            }
        }
    }
    lodepng::encode(filename.c_str(), uint8_image, width(), height());
}

void Image::debug_write() const {
    std::ostringstream ss;
    ss << "./Output/" <<  debugWriteNumber << ".png";
    std::string filename = ss.str();
    write(filename);
    debugWriteNumber++;
}

float Image::uint8_to_float(const unsigned char &in) {
    return ((float) in)/(255.0f);
}

unsigned char Image::float_to_uint8(const float &in) {
    float out = in;
    if (out < 0)
        out = 0;
    if (out > 1)
        out = 1;
    return (unsigned char) (255.0f*out);

}

// --------- HANDOUT  PS00 ------------------------------
// ------------------------------------------------------

void compareDimensions(const Image & im1, const Image & im2)  {
    if(im1.dimensions() != im2.dimensions())
        throw MismatchedDimensionsException();
    for (int i = 0; i < im1.dimensions(); i++ ) {
        if (im1.extent(i) != im2.extent(i))
            throw MismatchedDimensionsException();
    }
}


Image operator+ (const Image & im1, const Image & im2) {
    compareDimensions(im1, im2);
    long long total_pixels = im1.number_of_elements();

    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) + im2(i);
    }
    return output;
}

Image operator- (const Image & im1, const Image & im2) {
    compareDimensions(im1, im2);
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) - im2(i);
    }
    return output;
}

Image operator* (const Image & im1, const Image & im2) {
    compareDimensions(im1, im2);
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) * im2(i);
    }
    return output;

}

Image operator/ (const Image & im1, const Image & im2) {
    compareDimensions(im1, im2);
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        if (im2(i) == 0)
            throw DivideByZeroException();
        output(i) = im1(i) / im2(i);
    }
    return output;
}

Image operator+ (const Image & im1, const float & c) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) + c;
    }
    return output;
}

Image operator- (const Image & im1, const float & c) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) -  c;
    }
    return output;
}
Image operator* (const Image & im1, const float & c) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) * c;
    }
    return output;
}
Image operator/ (const Image & im1, const float & c) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    if (c==0)
        throw DivideByZeroException();
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i)/c;
    }
    return output;
}

Image operator+(const float & c, const Image & im1) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) + c;
    }
    return output;
}

Image operator- (const float & c, const Image & im1) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = c - im1(i);
    }
    return output;
}

Image operator* (const float & c, const Image & im1) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        output(i) = im1(i) * c;
    }
    return output;
}
Image operator/ (const float & c, const Image & im1) {
    long long total_pixels = im1.number_of_elements();
    Image output(im1.extent(0), im1.extent(1), im1.extent(2));
    for (int i = 0 ; i < total_pixels; i++) {
        if (im1(i) == 0)
            throw DivideByZeroException();
        output(i) = c/im1(i);
    }
    return output;
}
// ---------------- END of PS00 -------------------------------------
