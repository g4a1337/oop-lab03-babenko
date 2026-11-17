#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

class Complex {
    double real_ = 0, imag_ = 0;
    static Complex FromWide(long double real, long double imag);
    static int live_;
public:
    Complex();
    Complex(double real, double imag);
    Complex(const Complex& other);
    Complex& operator=(const Complex&) = default;
    ~Complex() noexcept;
    static int Live();
    void Init(double real, double imag);
    void Read();
    void Display() const;
    std::string toString() const;
    Complex add(const Complex& b) const;
    Complex sub(const Complex& b) const;
    Complex mul(const Complex& b) const;
    Complex div(const Complex& b) const;
    bool equ(const Complex& b) const;
    Complex conj() const;
};

int Complex::live_ = 0;
Complex::Complex() { ++live_; }
Complex::Complex(double real, double imag) { Init(real, imag); ++live_; }
Complex::Complex(const Complex& other) : real_(other.real_), imag_(other.imag_) { ++live_; }
Complex::~Complex() noexcept { --live_; }
int Complex::Live() { return live_; }
void Complex::Init(double real, double imag) {
    if (!std::isfinite(real) || !std::isfinite(imag))
        throw std::invalid_argument("Components must be finite");
    real_ = real; imag_ = imag;
}
void Complex::Read() {
    double real, imag;
    if (!(std::cin >> real >> imag)) throw std::runtime_error("Invalid input");
    Init(real, imag);
}
void Complex::Display() const { std::cout << toString() << '\n'; }
std::string Complex::toString() const {
    std::ostringstream out;
    out << real_ << (imag_ < 0 ? " - " : " + ") << std::fabs(imag_) << "i";
    return out.str();
}
Complex Complex::FromWide(long double real, long double imag) {
    const long double limit = std::numeric_limits<double>::max();
    if (!std::isfinite(real) || !std::isfinite(imag) ||
        std::fabs(real) > limit || std::fabs(imag) > limit)
        throw std::overflow_error("Complex result overflow");
    return Complex(static_cast<double>(real), static_cast<double>(imag));
}
Complex Complex::add(const Complex& b) const {
    return FromWide((long double)real_ + b.real_, (long double)imag_ + b.imag_);
}
Complex Complex::sub(const Complex& b) const {
    return FromWide((long double)real_ - b.real_, (long double)imag_ - b.imag_);
}
Complex Complex::mul(const Complex& b) const {
    return FromWide((long double)real_ * b.real_ - (long double)imag_ * b.imag_,
                    (long double)real_ * b.imag_ + (long double)imag_ * b.real_);
}
Complex Complex::div(const Complex& b) const {
    if (b.real_ == 0 && b.imag_ == 0) throw std::domain_error("Division by zero complex");
    long double d = (long double)b.real_ * b.real_ + (long double)b.imag_ * b.imag_;
    return FromWide(((long double)real_ * b.real_ + (long double)imag_ * b.imag_) / d,
                    ((long double)imag_ * b.real_ - (long double)real_ * b.imag_) / d);
}
bool Complex::equ(const Complex& b) const { return real_ == b.real_ && imag_ == b.imag_; }
Complex Complex::conj() const { return FromWide(real_, -imag_); }

void Require(bool ok) { if (!ok) throw std::logic_error("Self-check failed"); }
Complex Make(double real, double imag) { Complex z; z.Init(real, imag); return z; }
void SelfCheck() {
    Complex a = Make(3, 4), b = Make(1, -2);
    Require(a.add(b).equ(Make(4, 2)) && a.sub(b).equ(Make(2, 6)));
    Require(a.mul(b).equ(Make(11, -2)) && a.div(b).equ(Make(-1, 2)));
    Require(a.equ(a) && !a.equ(b) && a.conj().equ(Make(3, -4)));
    bool rejected = false;
    try { a.div(Make(0, 0)); } catch (const std::domain_error&) { rejected = true; }
    Require(rejected);
    rejected = false;
    try { a.Init(std::numeric_limits<double>::infinity(), 1); }
    catch (const std::invalid_argument&) { rejected = true; }
    Require(rejected && a.equ(Make(3, 4)));
    std::cout << "Arithmetic and validation self-checks: PASS\n";
}
void CheckLifecycle() {
    int before = Complex::Live();
    {
        Complex zero; Complex value(2, 3); Complex copy(value);
        Require(Complex::Live() == before + 3 && copy.equ(value));
        std::cout << "Default constructor: "; zero.Display();
        std::cout << "Parameter constructor: "; value.Display();
        std::cout << "Copy constructor: "; copy.Display();
        std::cout << "Live objects in scope: " << Complex::Live() - before << '\n';
    }
    Require(Complex::Live() == before);
    std::cout << "Destructor self-check: PASS; 3 scope objects destroyed\n";
}
int main() {
    try {
        CheckLifecycle();
        SelfCheck();
        Complex a, b;
        std::cout << "Enter real and imaginary parts of a: "; a.Read();
        std::cout << "Enter real and imaginary parts of b: "; b.Read();
        std::cout << "a = "; a.Display(); std::cout << "b = "; b.Display();
        std::cout << "add(a,b) = " << a.add(b).toString() << '\n';
        std::cout << "sub(a,b) = " << a.sub(b).toString() << '\n';
        std::cout << "mul(a,b) = " << a.mul(b).toString() << '\n';
        std::cout << "div(a,b) = " << a.div(b).toString() << '\n';
        std::cout << "equ(a,b) = " << std::boolalpha << a.equ(b) << '\n';
        std::cout << "conj(a) = " << a.conj().toString() << '\n';
        try { a.div(Make(0, 0)); }
        catch (const std::domain_error& e) { std::cout << "Expected error: " << e.what() << '\n'; }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n'; return 1;
    }
}
