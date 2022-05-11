# TinyShell
Operating System Principles Project by ©megaheart, ©thanhlam0241

## Note
- Ô **Lâm** ô sẽ làm việc với file `lam_funcs.cpp` (tạo file mới tại **Source Files**) và  `lam_funcs.h` (tạo file mới tại **Header Files**), nhớ tham khảo 2 file `shell_functions.cpp` và `shell_functions.h` mà tôi đã làm.
- Thay làm việc với `std::wcout`, `std::wcin`, `char`, `std::strcmp` ta sẽ làm việc lần lượt tương ứng với `std::wcout`, `std::wcin`, `TCHAR`, `std::wcscmp()`
- Chú ý function pointer type `ShellCommand`, khi tạo 1 *phương thức* thực thi 1 lệnh nào đó thì phải tạo theo cú pháp `int <tên phương thức>(TCHAR** cmdParts, int partCount)` (xem phương thức `int test(TCHAR** cmdParts, int partCount)` bên trong file `shell_functions.cpp`) với:
  - `cmdParts`: Lưu trữ tất cả thành phần của câu lệnh truyền vào, mỗi thành phần tương ứng với 1 chuỗi `TCHAR*`, vd: câu lệnh `proc run hello.exe` sẽ được chuyển về  `cmdParts` với `cmdParts[0] = "proc"`, `cmdParts[1] = "run"`, `cmdParts[2] = "hello.exe"`, 
  - `partCount`: số thành phần trong câu lệnh truyền vào, vd: câu lệnh `proc run hello.exe` sẽ có `partCount = 3`
  - Trả về mã thực thi với:
    -   0: Mã thực thi thành công
    -   1: `partCount` không hợp lệ với câu lệnh (quá nhiều, quá ít)
    -   2: Tham số truyền vào không hợp lệ
    -   3: ... chờ bổ sung ....
  - Chú ý hỗ trợ thực thi tham số `doc` trong câu lệnh (`cmdParts[1] == "doc"`)
