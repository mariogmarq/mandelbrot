#ifndef COMPLEX_H
#define COMPLEX_H

class Complex {
    private:
        double real;
        double imaginary;
    public:
        Complex(): real(0), imaginary(0) {};
        Complex(double x, double y): real(x), imaginary(y) {};
        Complex(double x): real(x), imaginary(0) {};
        double GetReal() const {return real;};
        double GetImg() const {return imaginary;};
};

Complex operator+(Complex oleft, Complex oright) {
    return Complex(oleft.GetReal()+oright.GetReal(), oleft.GetImg()+oright.GetImg());
}

Complex operator-(Complex oleft, Complex oright) {
    return Complex(oleft.GetReal()-oright.GetReal(), oleft.GetImg()-oright.GetImg());
}

Complex operator*(Complex oleft, Complex oright){
    return Complex(oright.GetReal()*oleft.GetReal()-oright.GetImg()*oright.GetImg(), oright.GetReal()*oleft.GetImg()+oleft.GetReal()*oright.GetImg());
}


#endif