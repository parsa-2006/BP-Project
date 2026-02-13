# BP-Project
A comprehensive C++ library for matrix and vector operations with automatic HTML report generation. Designed as a final project for the Fundamentals of Computer Science and Programming course at the University of Guilan (Iran).

✨ Features
Complete Matrix Operations:
Addition, subtraction, scalar multiplication
Matrix multiplication with dimension validation
Transpose operation
Recursive determinant calculation (up to 4×4)
Matrix inverse using adjugate method
Complete Vector Operations:
Addition, subtraction, scalar multiplication
Dot product (inner product)
Cross product (3D vectors only)
Magnitude calculation and normalization
Professional HTML Reporting:
Beautiful responsive design with RTL support for Persian content
Semantic HTML5 tags (<header>, <section>, <footer>)
Clear visual distinction between inputs (blue) and outputs (green)
Dimension labels for all matrices and vectors
Mathematical notation for operations (×, ·, ⁻¹, ᵀ)
Detailed error messages with Persian localization
Robust Error Handling:
Dimension mismatch in addition/subtraction
Invalid dimensions for multiplication
Non-square matrices for determinant/inverse
Singular matrices (determinant ≈ 0)
Invalid vector dimensions for cross product
🚀 Quick Start
Prerequisites
C++ compiler with C++11 support (g++, clang++, or MSVC)
Any modern web browser to view reports
Build & Run
bash
123456789101112131415
# Clone the repository
git clone https://github.com/your-username/matrix-vector-library.git
cd matrix-vector-library

# Compile the project
g++ -std=c++11 matrix_library.cpp demonstration.cpp -o matrix_demo

# Run the demonstration program
./matrix_demo  # Linux/macOS
matrix_demo.exe  # Windows

📊 Sample Output
The program generates a professional HTML report (report_sample.html) containing:


Sample report showing matrix operations with Persian interface
The report includes:
Student information section (name, student ID, university)
Separate sections for each operation with inputs and outputs
Visual representation of matrices/vectors with dimension labels
Mathematical symbols for operations (× for multiplication, · for dot product)
Error sections with clear Persian messages when operations fail
Validation section showing A × A⁻¹ ≈ I for inverse operations
📁 Project Structure
12345678
💡 Design Highlights
Object-Oriented Design: Separate Matrix and Vector classes with encapsulated data
Memory Safety: Uses std::vector for automatic memory management (no raw pointers)
Performance Optimizations:
Explicit implementations for 2×2 and 3×3 determinants (faster than recursive)
Direct array access in inner loops (avoiding getter/setter overhead)
Anonymous namespace for internal helper functions
Educational Focus: Clear, well-commented code suitable for learning linear algebra implementations
Cross-Platform: Works on Windows, Linux, and macOS with standard C++11
🎓 Academic Context
This project was developed as a final term project for the Fundamentals of Computer Science and Programming course (Department of Computer Science, University of Guilan, Iran - Winter 2026).
Key Requirements Met:
✅ Three interdependent source files (matrix_library.h, matrix_library.cpp, demonstration.cpp)
✅ All 7 required matrix operations implemented
✅ All 7 required vector operations implemented
✅ Professional HTML/CSS report generation with semantic tags
✅ Comprehensive error handling in Persian
✅ Dimension labeling and mathematical notation
✅ Test cases and complete documentation
⚠️ Limitations
Determinant calculation limited to 4×4 matrices (to prevent stack overflow in recursion)
Cross product only implemented for 3D vectors (mathematical constraint)
Floating-point precision limited to 4 decimal places in reports
Not optimized for very large matrices (>100×100)
📜 License
This project is for educational purposes only.
✅ Allowed: Studying the implementation approach, learning about OOP design in C++
❌ Not Allowed: Direct copying of code for academic submissions, commercial use
ℹ️ All code was written originally by the student without using external matrix libraries
Developed with ❤️ for the Department of Computer Science, University of Guilan
"Mathematics is the language in which God has written the universe." — Galileo Galilei
This library helps computers understand that language. 💻✨

