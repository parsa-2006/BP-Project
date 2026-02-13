#include "matrix_library.h"
#include <fstream>
#include <iostream>  

using namespace std;

int main() {
    string report = generateHtmlHeader();
    
    // ========== ماتریس‌های نمونه ==========
    Matrix A(2, 2);
    A.set(0,0,1); A.set(0,1,2);
    A.set(1,0,3); A.set(1,1,4);
    
    Matrix B(2, 2);
    B.set(0,0,5); B.set(0,1,6);
    B.set(1,0,7); B.set(1,1,8);
    
    Matrix C(3, 2);
    C.set(0,0,1); C.set(0,1,2);
    C.set(1,0,3); C.set(1,1,4);
    C.set(2,0,5); C.set(2,1,6);
    
    Matrix D(2, 3);
    D.set(0,0,1); D.set(0,1,2); D.set(0,2,3);
    D.set(1,0,4); D.set(1,1,5); D.set(1,2,6);
    
    Matrix E(3, 3);
    E.set(0,0,2); E.set(0,1,-1); E.set(0,2,0);
    E.set(1,0,-1); E.set(1,1,2); E.set(1,2,-1);
    E.set(2,0,0); E.set(2,1,-1); E.set(2,2,2);
    
    // ========== بردارهای نمونه ==========
    Vector v1({1, 2, 3});
    Vector v2({4, 5, 6});
    Vector v3({1, 0, -1});
    Vector v4({2, 3}); // برای تست خطا
    
    // ========== جمع ماتریس‌ها ==========
    string content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
                     A.toHtml("ماتریس A") + "<span class='op'>+</span>" + B.toHtml("ماتریس B") + "</div>";
    try {
        Matrix res = A.add(B);
        content += "<div class='output'><span class='output-title'>نتیجه جمع (A + B):</span>" + res.toHtml() + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("جمع ماتریس‌ها", content);
    
    // ========== ضرب ماتریس‌ها ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              C.toHtml("ماتریس C (3×2)") + "<span class='op'>×</span>" + D.toHtml("ماتریس D (2×3)") + "</div>";
    try {
        Matrix res = C.multiply(D);
        content += "<div class='output'><span class='output-title'>نتیجه ضرب (C × D):</span>" + res.toHtml("نتیجه (3×3)") + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ضرب ماتریس‌ها", content);
    
    // ========== ضرب اسکالر ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + 
              A.toHtml("ماتریس A") + "<div style='text-align:center; margin:15px 0; font-weight:bold;'>ضریب اسکالر: 2.5</div></div>";
    try {
        Matrix res = A.scalarMultiply(2.5);
        content += "<div class='output'><span class='output-title'>نتیجه ضرب اسکالر (2.5 × A):</span>" + res.toHtml() + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ضرب اسکالر در ماتریس", content);
    
    // ========== تفریق ماتریس‌ها ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              B.toHtml("ماتریس B") + "<span class='op'>−</span>" + A.toHtml("ماتریس A") + "</div>";
    try {
        Matrix res = B.subtract(A);
        content += "<div class='output'><span class='output-title'>نتیجه تفریق (B − A):</span>" + res.toHtml() + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("تفریق ماتریس‌ها", content);
    
    // ========== ترانهاده ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + D.toHtml("ماتریس D (2×3)") + "</div>";
    try {
        Matrix res = D.transpose();
        content += "<div class='output'><span class='output-title'>ترانهاده ماتریس (D<sup>T</sup>):</span>" + res.toHtml("نتیجه (3×2)") + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ترانهاده ماتریس", content);
    
    // ========== دترمینان ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + E.toHtml("ماتریس E (3×3)") + "</div>";
    try {
        double det = E.determinant();
        content += "<div class='output'>" + formatScalarResult("det(E)", det, "دترمینان ماتریس E") + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("محاسبه دترمینان (بازگشتی)", content);
    
    // ========== ماتریس معکوس ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + E.toHtml("ماتریس E (3×3)") + "</div>";
    try {
        Matrix inv = E.inverse();
        content += "<div class='output'><span class='output-title'>ماتریس معکوس (E<sup>-1</sup>):</span>" + inv.toHtml() + "</div>";
        
        // بررسی صحت: E × E⁻¹ ≈ I
        Matrix identity = E.multiply(inv);
        content += "<div style='background:#fff8e1; border:1px solid #ffd54f; border-radius:10px; padding:15px; margin-top:15px;'>";
        content += "<div style='font-weight:bold; color:#5d4037; margin-bottom:10px;'>بررسی صحت: E × E<sup>-1</sup> ≈ I (ماتریس همانی)</div>";
        content += identity.toHtml("ماتریس همانی تقریبی");
        content += "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("محاسبه ماتریس معکوس", content);
    
    // ========== جمع بردارها ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              v1.toHtml("بردار v₁", true) + "<span class='op'>+</span>" + v2.toHtml("بردار v₂", true) + "</div>";
    try {
        Vector res = v1.add(v2);
        content += "<div class='output'><span class='output-title'>نتیجه جمع (v₁ + v₂):</span>" + res.toHtml("بردار حاصل", true) + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("جمع بردارها", content);
    
    // ========== تفریق بردارها ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              v2.toHtml("بردار v₂", true) + "<span class='op'>−</span>" + v1.toHtml("بردار v₁", true) + "</div>";
    try {
        Vector res = v2.subtract(v1);
        content += "<div class='output'><span class='output-title'>نتیجه تفریق (v₂ − v₁):</span>" + res.toHtml("بردار حاصل", true) + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("تفریق بردارها", content);
    
    // ========== ضرب اسکالر در بردار ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + 
              v1.toHtml("بردار v₁", true) + "<div style='text-align:center; margin:15px 0; font-weight:bold;'>ضریب اسکالر: 3.0</div></div>";
    try {
        Vector res = v1.scalarMultiply(3.0);
        content += "<div class='output'><span class='output-title'>نتیجه ضرب اسکالر (3.0 × v₁):</span>" + res.toHtml("بردار حاصل", true) + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ضرب اسکالر در بردار", content);
    
    // ========== ضرب داخلی ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              v1.toHtml("بردار v₁", true) + "<span class='op'>·</span>" + v2.toHtml("بردار v₂", true) + "</div>";
    try {
        double dot = v1.dotProduct(v2);
        content += "<div class='output'>" + formatScalarResult("v₁ · v₂", dot, "ضرب داخلی بردارها") + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ضرب داخلی", content);
    
    // ========== ضرب خارجی ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              v1.toHtml("بردار v₁", true) + "<span class='op'>×</span>" + v3.toHtml("بردار v₃", true) + "</div>";
    try {
        Vector res = v1.crossProduct(v3);
        content += "<div class='output'><span class='output-title'>نتیجه ضرب خارجی (v₁ × v₃):</span>" + res.toHtml("بردار حاصل", true) + "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("ضرب خارجی", content);
    
    // ========== تست خطا در ضرب خارجی ==========
    content = "<div class='input'><span class='input-title'>ورودی‌ها:</span>" + 
              v1.toHtml("بردار v₁ (3 بعدی)", true) + "<span class='op'>×</span>" + v4.toHtml("بردار v₄ (2 بعدی)", true) + "</div>";
    try {
        Vector res = v1.crossProduct(v4);
        content += "<div class='output'>" + res.toHtml("نتیجه", true) + "</div>";
    } catch (MatrixException& e) {
        content += "<div class='output-title' style='text-align:center; margin-bottom:15px;'>نتیجه تست خطا:</div>";
        content += formatError(e.what());
    }
    report += formatSection("تست خطا: ضرب خارجی با ابعاد نامعتبر", content);
    
    // ========== اندازه و نرمال‌سازی ==========
    content = "<div class='input'><span class='input-title'>ورودی:</span>" + v1.toHtml("بردار v₁", true) + "</div>";
    try {
        double mag = v1.magnitude();
        Vector norm = v1.normalize();
        content += "<div class='output'>";
        content += formatScalarResult("||v₁||", mag, "اندازه بردار (Magnitude)");
        content += "<div style='margin:20px 0; text-align:center; font-weight:bold; color:#1a5276;'>بردار نرمال‌شده (v̂₁ = v₁ / ||v₁||):</div>";
        content += norm.toHtml("بردار یکه", true);
        content += "</div>";
    } catch (MatrixException& e) {
        content += formatError(e.what());
    }
    report += formatSection("اندازه و نرمال‌سازی بردار", content);
    
    // ذخیره گزارش نهایی
    report += generateHtmlFooter();
    ofstream file("report_sample.html", ios::out | ios::trunc);
    file << report;
    file.close();
    
    // ایجاد فایل تست‌کیس‌ها
    ofstream testFile("test_cases.txt");
    testFile << "نمونه‌های تست برای کتابخانه عملیات ماتریسی و برداری\n";
    testFile << "==================================================\n\n";
    
    testFile << "1. جمع ماتریس‌ها:\n";
    testFile << "   A = [[1, 2], [3, 4]]\n";
    testFile << "   B = [[5, 6], [7, 8]]\n";
    testFile << "   نتیجه: [[6, 8], [10, 12]]\n\n";
    
    testFile << "2. ضرب ماتریس‌ها:\n";
    testFile << "   C = [[1, 2], [3, 4], [5, 6]] (3×2)\n";
    testFile << "   D = [[1, 2, 3], [4, 5, 6]] (2×3)\n";
    testFile << "   نتیجه: [[9, 12, 15], [19, 26, 33], [29, 40, 51]] (3×3)\n\n";
    
    testFile << "3. دترمینان ماتریس:\n";
    testFile << "   E = [[2, -1, 0], [-1, 2, -1], [0, -1, 2]]\n";
    testFile << "   نتیجه: 4.0000\n\n";
    
    testFile << "4. ماتریس معکوس:\n";
    testFile << "   E = [[2, -1, 0], [-1, 2, -1], [0, -1, 2]]\n";
    testFile << "   نتیجه: [[0.75, 0.50, 0.25], [0.50, 1.00, 0.50], [0.25, 0.50, 0.75]]\n\n";
    
    testFile << "5. ضرب داخلی بردارها:\n";
    testFile << "   v₁ = [1, 2, 3]\n";
    testFile << "   v₂ = [4, 5, 6]\n";
    testFile << "   نتیجه: 32.0000\n\n";
    
    testFile << "6. ضرب خارجی بردارها:\n";
    testFile << "   v₁ = [1, 2, 3]\n";
    testFile << "   v₃ = [1, 0, -1]\n";
    testFile << "   نتیجه: [-2.00, 4.00, -2.00]\n\n";
    
    testFile << "7. تست خطا - ضرب خارجی با ابعاد نامعتبر:\n";
    testFile << "   v₁ = [1, 2, 3] (3 بعدی)\n";
    testFile << "   v₄ = [2, 3] (2 بعدی)\n";
    testFile << "   نتیجه: خطا - ضرب خارجی فقط برای بردارهای سه‌بعدی تعریف شده است\n\n";
    
    testFile << "8. اندازه و نرمال‌سازی:\n";
    testFile << "   v₁ = [1, 2, 3]\n";
    testFile << "   اندازه: 3.7417\n";
    testFile << "   بردار نرمال‌شده: [0.2673, 0.5345, 0.8018]\n";
    testFile.close();
    
    // ایجاد فایل README
    ofstream readme("README.txt");
    readme << "کتابخانه عملیات ماتریسی و برداری\n";
    readme << "==================================\n\n";
    
    readme << "دانشجو: سید پارسا موسوی نسب (1404010121040)\n";
    readme << "دانشگاه گیلان - گروه علوم کامپیوتر\n";
    readme << "درس: مبانی کامپیوتر و برنامه‌نویسی - ترم بهمن 1404\n\n";
    
    readme << "فایل‌های پروژه:\n";
    readme << "----------------\n";
    readme << "- matrix_library.h   : فایل هدر شامل اعلان کلاس‌ها و توابع\n";
    readme << "- matrix_library.cpp : پیاده‌سازی کامل کلاس‌ها و توابع\n";
    readme << "- demonstration.cpp  : برنامه نمایشی که گزارش HTML تولید می‌کند\n";
    readme << "- report_sample.html : نمونه خروجی برنامه (تولید شده توسط demonstration.cpp)\n";
    readme << "- test_cases.txt     : نمونه‌های تست برای عملیات مختلف\n";
    readme << "- README.txt         : این فایل مستندات\n\n";
    
    readme << "نحوه کامپایل و اجرا:\n";
    readme << "---------------------\n";
    readme << "1. کامپایل فایل‌ها با کامپایلر g++:\n";
    readme << "   g++ -std=c++11 matrix_library.cpp demonstration.cpp -o matrix_demo\n\n";
    readme << "2. اجرای برنامه:\n";
    readme << "   ./matrix_demo\n\n";
    readme << "3. پس از اجرا، فایل‌های زیر تولید می‌شوند:\n";
    readme << "   - report_sample.html : گزارش کامل عملیات با فرمت HTML\n";
    readme << "   - test_cases.txt     : نمونه‌های تست برای اعتبارسنجی\n\n";
    
    readme << "محدودیت‌های پیاده‌سازی:\n";
    readme << "------------------------\n";
    readme << "1. دترمینان: محاسبه بازگشتی فقط تا ماتریس 4×4 پشتیبانی می‌شود (جلوگیری از سرریز پشته)\n";
    readme << "2. ضرب خارجی: فقط برای بردارهای دقیقاً سه‌بعدی پیاده‌سازی شده است\n";
    readme << "3. ماتریس معکوس: فقط برای ماتریس‌های مربعی و معکوس‌پذیر (دترمینان ≠ 0)\n";
    readme << "4. دقت اعشاری: نمایش ماتریس‌ها با 2 رقم، نتایج عددی با 4 رقم اعشار\n\n";
    
    readme << "ویژگی‌های گزارش HTML:\n";
    readme << "-----------------------\n";
    readme << "- استفاده از تگ‌های معنایی HTML5 (header, section, footer)\n";
    readme << "- طراحی واکنش‌گرا برای نمایش مناسب در موبایل و دسکتاپ\n";
    readme << "- تمایز واضح بین ورودی‌ها (پس‌زمینه آبی کمرنگ) و خروجی‌ها (پس‌زمینه سبز کمرنگ)\n";
    readme << "- برچسب‌گذاری ابعاد برای تمام ماتریس‌ها و بردارها در کنار هر ماتریس/بردار\n";
    readme << "- استفاده از نمادهای ریاضی استاندارد (× برای ضرب ماتریسی، · برای ضرب داخلی)\n";
    readme << "- نمایش پیام‌های خطا با بک‌گراند قرمز و آیکون ❌ برای جلب توجه\n";
    readme << "- پشتیبانی کامل از زبان فارسی با جهت‌دهی راست به چپ (dir=\"rtl\")\n\n";
    
    readme << "انتخاب‌های پیاده‌سازی مهم:\n";
    readme << "---------------------------\n";
    readme << "1. استفاده از کلاس‌های جداگانه Matrix و Vector برای رعایت اصول شیءگرا\n";
    readme << "2. پیاده‌سازی صریح دترمینان 2x2 و 3x3 برای بهبود عملکرد (سریع‌تر از بازگشتی)\n";
    readme << "3. مدیریت خطا با پیام‌های فارسی دقیق شامل ابعاد و شاخص‌های خطا\n";
    readme << "4. تولید گزارش با استایل داخلی (Inline CSS) برای استقلال کامل فایل HTML\n";
    readme << "5. استفاده از رفرنس‌ها برای جلوگیری از کپی غیرضروری داده‌ها در پارامترها\n";
    readme.close();
    
 cout << "\n✅ Project executed successfully!\n";
cout << "Generated files:\n";
cout << "  • report_sample.html  - Interactive HTML report\n";
cout << "  • test_cases.txt      - Test cases for validation\n";
cout << "  • README.txt          - Project documentation\n";
cout << "\nOpen report_sample.html in your web browser to view the report.\n" << endl;
    
    return 0;
}