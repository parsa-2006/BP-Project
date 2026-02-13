#include "matrix_library.h"

using namespace std;

// ========== توابع کمکی داخلی ==========
namespace {
    // اعتبارسنجی سریع ابعاد
    inline void validateDimensions(int r, int c, const string& context) {
        if (r <= 0 || c <= 0) 
            throw MatrixException(context + " ابعاد ماتریس باید اعداد مثبت باشند");
    }
    
    // محاسبه دترمینان 2x2 به صورت مستقیم (بهینه)
    inline double det2x2(const vector<vector<double>>& m) {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
    
    // محاسبه دترمینان 3x3 به صورت مستقیم (بهینه)
    inline double det3x3(const vector<vector<double>>& m) {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
             - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
             + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }
}

// ========== پیاده‌سازی کلاس Matrix ==========

Matrix::Matrix(int r, int c) : rows(r), cols(c) {
    validateDimensions(r, c, "ساخت ماتریس: ");
    data.assign(r, vector<double>(c, 0.0));
}

Matrix::Matrix(const vector<vector<double>>& d) : data(d), rows(d.size()), cols(d.empty() ? 0 : d[0].size()) {
    if (rows == 0 || cols == 0) throw MatrixException("ماتریس نمی‌تواند خالی باشد");
    for (size_t i = 0; i < data.size(); i++)
        if ((int)data[i].size() != cols) 
            throw MatrixException("سطر " + to_string(i+1) + " با سایر سطرها هم‌اندازه نیست");
}

double Matrix::get(int i, int j) const {
    if (i < 0 || i >= rows || j < 0 || j >= cols) 
        throw MatrixException("خطا: شاخص [" + to_string(i) + "][" + to_string(j) + "] خارج از محدوده ماتریس " + to_string(rows) + "×" + to_string(cols) + " است");
    return data[i][j];
}

void Matrix::set(int i, int j, double value) {
    if (i < 0 || i >= rows || j < 0 || j >= cols) 
        throw MatrixException("خطا: شاخص [" + to_string(i) + "][" + to_string(j) + "] خارج از محدوده ماتریس " + to_string(rows) + "×" + to_string(cols) + " است");
    data[i][j] = value;
}

Matrix Matrix::add(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) 
        throw MatrixException("خطا: ابعاد ماتریس‌ها برای جمع نامعتبر است (" + to_string(rows) + "×" + to_string(cols) + " با " + to_string(other.rows) + "×" + to_string(other.cols) + ")");
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] + other.data[i][j];
    return result;
}

Matrix Matrix::subtract(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) 
        throw MatrixException("خطا: ابعاد ماتریس‌ها برای تفریق نامعتبر است (" + to_string(rows) + "×" + to_string(cols) + " با " + to_string(other.rows) + "×" + to_string(other.cols) + ")");
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] - other.data[i][j];
    return result;
}

Matrix Matrix::multiply(const Matrix& other) const {
    if (cols != other.rows) 
        throw MatrixException("خطا: تعداد ستون‌های ماتریس اول (" + to_string(cols) + ") با تعداد سطرهای ماتریس دوم (" + to_string(other.rows) + ") برابر نیست");
    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < other.cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < cols; k++)
                sum += data[i][k] * other.data[k][j];
            result.data[i][j] = sum;
        }
    return result;
}

Matrix Matrix::scalarMultiply(double scalar) const {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] * scalar;
    return result;
}

Matrix Matrix::transpose() const {
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[j][i] = data[i][j];
    return result;
}

double Matrix::determinant() const {
    if (rows != cols) throw MatrixException("خطا: محاسبه دترمینان فقط برای ماتریس‌های مربعی ممکن است");
    if (rows == 1) return data[0][0];
    if (rows == 2) return det2x2(data);
    if (rows == 3) return det3x3(data);
    if (rows > 4) throw MatrixException("خطا: محاسبه دترمینان به صورت بازگشتی فقط تا ماتریس 4×4 پشتیبانی می‌شود");
    
    // روش بازگشتی برای 4x4
    double det = 0.0;
    for (int j = 0; j < cols; j++) {
        vector<vector<double>> minor(rows-1, vector<double>(cols-1));
        for (int i = 1; i < rows; i++) {
            int col = 0;
            for (int k = 0; k < cols; k++) {
                if (k != j) {
                    minor[i-1][col] = data[i][k];
                    col++;
                }
            }
        }
        Matrix m(minor);
        double sign = (j % 2 == 0) ? 1.0 : -1.0;
        det += sign * data[0][j] * m.determinant();
    }
    return det;
}

Matrix Matrix::inverse() const {
    if (rows != cols) throw MatrixException("خطا: معکوس فقط برای ماتریس‌های مربعی تعریف شده است");
    double det = determinant();
    if (abs(det) < 1e-10) throw MatrixException("خطا: ماتریس معکوس‌ناپذیر است (دترمینان ≈ 0)");
    
    // ماتریس 1x1
    if (rows == 1) {
        Matrix inv(1, 1);
        inv.data[0][0] = 1.0 / data[0][0];
        return inv;
    }
    
    // ماتریس 2x2 - روش مستقیم (بهینه)
    if (rows == 2) {
        Matrix inv(2, 2);
        inv.data[0][0] =  data[1][1] / det;
        inv.data[0][1] = -data[0][1] / det;
        inv.data[1][0] = -data[1][0] / det;
        inv.data[1][1] =  data[0][0] / det;
        return inv;
    }
    
    // ماتریس‌های بزرگتر - روش ماتریس الحاقی
    Matrix adj(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            vector<vector<double>> minor(rows-1, vector<double>(cols-1));
            int mi = 0;
            for (int ii = 0; ii < rows; ii++) {
                if (ii == i) continue;
                int mj = 0;
                for (int jj = 0; jj < cols; jj++) {
                    if (jj == j) continue;
                    minor[mi][mj] = data[ii][jj];
                    mj++;
                }
                mi++;
            }
            Matrix m(minor);
            double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            adj.data[j][i] = sign * m.determinant();
        }
    }
    return adj.scalarMultiply(1.0 / det);
}

string Matrix::toString() const {
    ostringstream oss;
    oss << fixed << setprecision(2);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            oss << data[i][j];
            if (j < cols-1) oss << "\t";
        }
        if (i < rows-1) oss << "\n";
    }
    return oss.str();
}

string Matrix::toHtml(string title, bool showDimensions) const {
    ostringstream oss;
    oss << "<div class='matrix-box'>";
    if (!title.empty()) oss << "<div class='matrix-title'>" << title << "</div>";
    oss << "<table class='matrix'>";
    for (int i = 0; i < rows; i++) {
        oss << "<tr>";
        for (int j = 0; j < cols; j++) 
            oss << "<td>" << fixed << setprecision(2) << data[i][j] << "</td>";
        oss << "</tr>";
    }
    oss << "</table>";
    if (showDimensions) oss << "<div class='dim'>(" << rows << "×" << cols << ")</div>";
    oss << "</div>";
    return oss.str();
}

// ========== پیاده‌سازی کلاس Vector ==========

Vector::Vector(const vector<double>& d) : data(d) {
    if (data.empty()) throw MatrixException("خطا: بردار نمی‌تواند خالی باشد");
}

double Vector::get(int i) const {
    if (i < 0 || i >= (int)data.size()) 
        throw MatrixException("خطا: شاخص [" + to_string(i) + "] خارج از محدوده بردار با اندازه " + to_string(data.size()) + " است");
    return data[i];
}

void Vector::set(int i, double value) {
    if (i < 0 || i >= (int)data.size()) 
        throw MatrixException("خطا: شاخص [" + to_string(i) + "] خارج از محدوده بردار با اندازه " + to_string(data.size()) + " است");
    data[i] = value;
}

Vector Vector::add(const Vector& other) const {
    if (data.size() != other.data.size()) 
        throw MatrixException("خطا: ابعاد بردارها برای جمع نامعتبر است (" + to_string(data.size()) + " با " + to_string(other.data.size()) + ")");
    vector<double> res(data.size());
    for (size_t i = 0; i < data.size(); i++) res[i] = data[i] + other.data[i];
    return Vector(res);
}

Vector Vector::subtract(const Vector& other) const {
    if (data.size() != other.data.size()) 
        throw MatrixException("خطا: ابعاد بردارها برای تفریق نامعتبر است (" + to_string(data.size()) + " با " + to_string(other.data.size()) + ")");
    vector<double> res(data.size());
    for (size_t i = 0; i < data.size(); i++) res[i] = data[i] - other.data[i];
    return Vector(res);
}

Vector Vector::scalarMultiply(double scalar) const {
    vector<double> res(data.size());
    for (size_t i = 0; i < data.size(); i++) res[i] = data[i] * scalar;
    return Vector(res);
}

double Vector::dotProduct(const Vector& other) const {
    if (data.size() != other.data.size()) 
        throw MatrixException("خطا: ابعاد بردارها برای ضرب داخلی نامعتبر است (" + to_string(data.size()) + " با " + to_string(other.data.size()) + ")");
    double sum = 0.0;
    for (size_t i = 0; i < data.size(); i++) sum += data[i] * other.data[i];
    return sum;
}

Vector Vector::crossProduct(const Vector& other) const {
    if (data.size() != 3 || other.data.size() != 3) 
        throw MatrixException("خطا: ضرب خارجی فقط برای بردارهای سه‌بعدی تعریف شده است (ابعاد ورودی: " + to_string(data.size()) + " و " + to_string(other.data.size()) + ")");
    vector<double> res(3);
    res[0] = data[1]*other.data[2] - data[2]*other.data[1];
    res[1] = data[2]*other.data[0] - data[0]*other.data[2];
    res[2] = data[0]*other.data[1] - data[1]*other.data[0];
    return Vector(res);
}

double Vector::magnitude() const {
    double sum = 0.0;
    for (double v : data) sum += v * v;
    return sqrt(sum);
}

Vector Vector::normalize() const {
    double mag = magnitude();
    if (abs(mag) < 1e-10) throw MatrixException("خطا: نمی‌توان بردار صفر را نرمال‌سازی کرد");
    vector<double> res(data.size());
    for (size_t i = 0; i < data.size(); i++) res[i] = data[i] / mag;
    return Vector(res);
}

string Vector::toString() const {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < data.size(); i++) {
        oss << fixed << setprecision(2) << data[i];
        if (i < data.size()-1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

string Vector::toHtml(string title, bool asRow, bool showDimensions) const {
    ostringstream oss;
    oss << "<div class='vector-box'>";
    if (!title.empty()) oss << "<div class='vector-title'>" << title << "</div>";
    oss << "<table class='vector'>";
    if (asRow) {
        oss << "<tr>";
        for (double v : data) oss << "<td>" << fixed << setprecision(2) << v << "</td>";
        oss << "</tr>";
    } else {
        for (double v : data) oss << "<tr><td>" << v << "</td></tr>";
    }
    oss << "</table>";
    if (showDimensions) 
        oss << "<div class='dim'>(" << data.size() << (asRow ? "×1" : " عضو") << ")</div>";
    oss << "</div>";
    return oss.str();
}

// ========== توابع تولید گزارش HTML ==========

string generateHtmlHeader() {
    return R"(
<!DOCTYPE html>
<html dir="rtl" lang="fa">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>گزارش عملیات ماتریسی و برداری</title>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body { 
            font-family: 'Tahoma', 'Vazir', sans-serif; 
            background: linear-gradient(135deg, #f5f7fa 0%, #e4edf5 100%);
            color: #2c3e50; 
            line-height: 1.6; 
            padding: 20px; 
            max-width: 1100px; 
            margin: 0 auto; 
        }
        header { 
            background: linear-gradient(120deg, #1a5276 0%, #2e86c1 100%);
            color: white; 
            padding: 25px; 
            border-radius: 15px; 
            text-align: center; 
            margin-bottom: 30px;
            box-shadow: 0 6px 15px rgba(0,0,0,0.15);
        }
        h1 { font-size: 32px; margin-bottom: 10px; text-shadow: 0 2px 4px rgba(0,0,0,0.2); }
        .subtitle { font-size: 18px; opacity: 0.95; margin-top: 8px; }
        .student-info { 
            background: rgba(255,255,255,0.15); 
            display: inline-block; 
            padding: 12px 25px; 
            border-radius: 30px; 
            margin-top: 15px;
            font-size: 16px;
        }
        section { 
            background: white; 
            border-radius: 15px; 
            padding: 25px; 
            margin-bottom: 30px; 
            box-shadow: 0 4px 12px rgba(0,0,0,0.08);
            transition: transform 0.3s ease;
        }
        section:hover { transform: translateY(-3px); }
        h2 { 
            color: #2e86c1; 
            margin-bottom: 20px; 
            padding-bottom: 12px; 
            border-bottom: 2px solid #3498db; 
            font-size: 24px;
        }
        .input { 
            background: #e3f2fd; 
            border: 1px solid #bbdefb; 
            border-radius: 12px; 
            padding: 20px; 
            margin: 20px 0;
        }
        .output { 
            background: #e8f5e9; 
            border: 1px solid #c8e6c9; 
            border-radius: 12px; 
            padding: 20px; 
            margin: 20px 0;
        }
        .input-title, .output-title { 
            font-weight: bold; 
            color: #1a5276; 
            margin-bottom: 15px; 
            font-size: 18px;
            display: block;
        }
        .matrix-box, .vector-box { 
            display: inline-block; 
            margin: 12px; 
            text-align: center; 
            vertical-align: middle;
        }
        .matrix-title, .vector-title { 
            font-weight: bold; 
            color: #2874a6; 
            margin-bottom: 8px; 
            font-size: 16px;
        }
        .matrix, .vector { 
            border-collapse: collapse; 
            margin: 10px auto;
            min-width: 80px;
        }
        .matrix td, .vector td { 
            border: 2px solid #3498db; 
            padding: 10px 15px; 
            background: white;
            min-width: 50px;
            font-weight: 500;
        }
        .matrix tr:nth-child(even) td, .vector tr:nth-child(even) td { 
            background: #f8fbff; 
        }
        .dim { 
            color: #7f8c8d; 
            font-size: 14px; 
            margin-top: 6px; 
            font-style: italic;
            font-weight: normal;
        }
        .op { 
            font-size: 28px; 
            color: #2e86c1; 
            margin: 0 15px; 
            font-weight: bold; 
            display: inline-block; 
            vertical-align: middle;
            min-width: 40px;
            text-align: center;
        }
        .scalar { 
            background: linear-gradient(90deg, #e3f2fd 0%, #bbdefb 100%);
            border-left: 5px solid #2196f3; 
            padding: 18px; 
            margin: 20px 0; 
            border-radius: 0 10px 10px 0; 
            text-align: center; 
            font-weight: bold; 
            font-size: 20px;
            color: #1a5276;
        }
        .error { 
            background: #ffebee; 
            color: #c62828; 
            border: 2px solid #ef9a9a; 
            border-radius: 12px; 
            padding: 20px; 
            margin: 20px 0; 
            text-align: center; 
            font-weight: bold;
            font-size: 18px;
        }
        footer { 
            text-align: center; 
            margin-top: 40px; 
            padding: 25px; 
            color: #7f8c8d; 
            border-top: 1px solid #e0e0e0;
            font-size: 16px;
            line-height: 1.7;
        }
        @media (max-width: 768px) {
            .matrix-box, .vector-box { display: block; margin: 15px auto; }
            section { padding: 18px; }
            h1 { font-size: 26px; }
            h2 { font-size: 20px; }
        }
    </style>
</head>
<body>
<header>
    <h1>📚 کتابخانه عملیات ماتریسی و برداری</h1>
    <div class="subtitle">پروژه پایان ترم درس مبانی کامپیوتر و برنامه‌نویسی</div>
    <div class="student-info">
        👤 <strong>نام دانشجو:</strong> سید پارسا موسوی نسب&nbsp; | &nbsp; 
        🆔 <strong>شماره دانشجویی:</strong> 1404010121040 &nbsp; | &nbsp;
        🏫 <strong>دانشگاه گیلان - گروه علوم کامپیوتر</strong>
    </div>
</header>
)";
}

string generateHtmlFooter() {
    return R"(
<footer>
    <p>✅ گزارش تولید شده توسط کتابخانه عملیات ماتریسی و برداری</p>
    <p>🎓 پروژه پایان ترم درس مبانی کامپیوتر و برنامه‌نویسی - دانشگاه گیلان</p>
</footer>
</body>
</html>
)";
}

string formatScalarResult(string operation, double result, string title) {
    ostringstream oss;
    oss << "<div class='scalar'>";
    if (!title.empty()) oss << title << "<br>";
    oss << "<span style='font-size:22px; color:#2e86c1'>" << operation << "</span> = " 
        << fixed << setprecision(4) << result;
    oss << "</div>";
    return oss.str();
}

string formatError(string message) {
    return "<div class='error'>❌ " + message + "</div>";
}

string formatSection(string title, string content) {
    ostringstream oss;
    oss << "<section><h2>" << title << "</h2>" << content << "</section>";
    return oss.str();
}