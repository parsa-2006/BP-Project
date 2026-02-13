#ifndef MATRIX_LIBRARY_H
#define MATRIX_LIBRARY_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

// کلاس استثنا برای مدیریت خطاها با پیام‌های فارسی
class MatrixException : public runtime_error {
public:
    explicit MatrixException(const string& message) : runtime_error(message) {}
};

// کلاس ماتریس
class Matrix {
private:
    vector<vector<double>> data;
    int rows, cols;
    
public:
    // سازنده‌ها
    Matrix(int r, int c);
    Matrix(const vector<vector<double>>& d);
    
    // متدهای دسترسی
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    double get(int i, int j) const;
    void set(int i, int j, double value);
    
    // عملیات ماتریسی
    Matrix add(const Matrix& other) const;
    Matrix subtract(const Matrix& other) const;
    Matrix multiply(const Matrix& other) const;
    Matrix scalarMultiply(double scalar) const;
    Matrix transpose() const;
    double determinant() const;
    Matrix inverse() const;
    
    // تبدیل به رشته و HTML
    string toString() const;
    string toHtml(string title = "", bool showDimensions = true) const;
};

// کلاس بردار
class Vector {
private:
    vector<double> data;
    
public:
    // سازنده
    explicit Vector(const vector<double>& d);
    
    // متدهای دسترسی
    int getSize() const { return data.size(); }
    double get(int i) const;
    void set(int i, double value);
    
    // عملیات برداری
    Vector add(const Vector& other) const;
    Vector subtract(const Vector& other) const;
    Vector scalarMultiply(double scalar) const;
    double dotProduct(const Vector& other) const;
    Vector crossProduct(const Vector& other) const;
    double magnitude() const;
    Vector normalize() const;
    
    // تبدیل به رشته و HTML
    string toString() const;
    string toHtml(string title = "", bool asRow = false, bool showDimensions = true) const;
};

// توابع تولید گزارش HTML
string generateHtmlHeader();
string generateHtmlFooter();
string formatScalarResult(string operation, double result, string title = "");
string formatError(string message);
string formatSection(string title, string content);

#endif