# Hệ thống ví điểm thưởng C++

## Giới thiệu dự án
- Dự án này xây dựng một hệ thống ví điện tử điểm thưởng bằng ngôn ngữ C++, giao diện dòng lệnh.
- Hệ thống hỗ trợ đăng ký, đăng nhập, quản lý ví, chuyển điểm, xác thực OTP, phân quyền người dùng (admin/người thường).
- Ghi lịch sử giao dịch, tự động sao lưu/phục hồi dữ liệu và các yêu cầu bảo mật khác.

## Thành viên nhóm

| Họ tên              | Mã SV      | File đảm nhiệm                                  | Mô tả công việc cụ thể                                                                                                                       |
|---------------------|------------|-------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------|
| **Trần Kim Nga**    | K23DTCN608 | utils.h, utils.cpp                              | Xử lý các chức năng tiện ích: băm mật khẩu SHA256, kiểm tra mật khẩu, sinh mật khẩu tự động, sao lưu và phục hồi dữ liệu (backup/restore).   |
| **Từ Ngọc Thủy**    | K24DTCN565 | wallet.h, wallet.cpp                            | Quản lý ví điểm: tạo ví cho user, ví tổng, lưu/đọc số dư, tìm kiếm ví theo username/ID, đảm bảo tính nhất quán khi chuyển điểm/giao dịch.    |
| **Lại Văn Thể**     | K24DTCN670 | user.h, user.cpp                                | Quản lý tài khoản: đăng ký, đăng nhập, đổi mật khẩu, kiểm tra trùng số điện thoại, phân quyền, lưu/đọc danh sách user, xử lý mật khẩu tự sinh|
| **Hoàng Thanh Sơn** | K24DTCN419 | transaction.h, transaction.cpp; otp.h, otp.cpp; | Xử lý giao dịch chuyển điểm, ghi và hiển thị lịch sử giao dịch; sinh và xác thực OTP; giao tiếp xác thực OTP qua pipe; xây dựng menu chính,  |
|                     |            | ipc.h, ipc.cpp; main.cpp; otp_confirm.cpp       | điều phối toàn bộ chương trình, xử lý logic phân quyền, tích hợp các module.                                                                 |

## Đặc tả chức năng
- Đăng ký/đăng nhập/đổi mật khẩu
- Mật khẩu tự sinh, bắt đổi lần đầu
- OTP xác thực chuyển điểm
- Quản lý ví, giao dịch, lịch sử
- Phân quyền (admin/người dùng)
- Sao lưu/phục hồi dữ liệu

## Hướng dẫn build và chạy
- Yêu cầu: MinGW/C++ trên Windows
- Build: `g++ *.cpp -o main.exe`
- Chạy: `main.exe`
- Chạy tiến trình OTP: `otp_confirm.exe`
- Thư mục data/ phải nằm cùng cấp với main.exe

## Mô tả input/output
- File users.txt: thông tin user
- File wallets.txt: thông tin ví
- File transactions.txt: lịch sử giao dịch

## Mô tả chi tiết input/output của chương trình

### 1. Tổng quan

- **Giao diện:** Dòng lệnh (console), nhập xuất qua bàn phím và màn hình.
- **Input:** Người dùng nhập thông tin theo hướng dẫn trên màn hình.
- **Output:** Chương trình in thông báo, kết quả, trạng thái, lịch sử giao dịch ra màn hình.

---

### 2. Input/Output cho từng chức năng

#### A. Đăng ký tài khoản

- **Input:**
  - Tên đăng nhập (username)
  - Số điện thoại
  - Họ tên
  - Mật khẩu (có thể để trống để hệ thống tự sinh)
  - Phân quyền (USER/ADMIN)
- **Output:**
  - Thông báo đăng ký thành công/thất bại
  - Nếu để trống mật khẩu: in mật khẩu tự sinh ra màn hình
  - Thông báo số điện thoại đã tồn tại nếu trùng
  - Thông báo username đã tồn tại nếu trùng (tự động thêm số thứ tự phía sau)
  - Thông báo nếu mật khẩu bị trùng hash với user khác

#### B. Đăng nhập

- **Input:**
  - Tên đăng nhập (username)
  - Mật khẩu
- **Output:**
  - Thông báo đăng nhập thành công/thất bại
  - Nếu là mật khẩu tự sinh: yêu cầu đổi mật khẩu mới
  - Hiển thị thông tin ví: username, số điện thoại, số dư

#### C. Đổi mật khẩu

- **Input:**
  - Mật khẩu mới (nhập 2 lần để xác nhận)
- **Output:**
  - Thông báo đổi mật khẩu thành công/thất bại
  - Thông báo nếu trùng hash với mật khẩu của user khác

#### D. Nạp điểm từ ví tổng (chỉ admin)

- **Input:**
  - Tên đăng nhập người nhận
  - Số điểm muốn nạp
- **Output:**
  - Thông báo nạp thành công/thất bại
  - Thông báo nếu ví tổng không đủ điểm hoặc user nhận không tồn tại

#### E. Chuyển điểm giữa các ví

- **Input:**
  - ID ví người nhận
  - Số điểm muốn chuyển
  - Mã OTP xác thực (nhập từ cửa sổ otp_confirm.exe)
- **Output:**
  - Thông báo chuyển thành công/thất bại
  - Thông báo nếu số dư không đủ, ví nhận không tồn tại, OTP sai hoặc giao dịch bị từ chối

#### F. Xem số dư ví

- **Input:**  
  - (Chọn menu, không cần nhập thêm)
- **Output:**  
  - Hiển thị số dư hiện tại của ví

#### G. Xem lịch sử giao dịch

- **Input:**  
  - (Chọn menu, không cần nhập thêm)
- **Output:**  
  - In ra danh sách giao dịch liên quan đến ví (từ, đến, số điểm, trạng thái, thời gian hh:mm dd/mm/yyyy)

#### H. Phục hồi dữ liệu

- **Input:**  
  - (Chọn menu, không cần nhập thêm)
- **Output:**  
  - Thông báo phục hồi thành công
  - Dữ liệu các file users.txt, wallets.txt, transactions.txt được khôi phục từ bản backup (.bak)

---

### 3. Input/Output của các file dữ liệu

#### a) users.txt

- **Khi ghi:**  
  - Mỗi dòng:  
    `username|password_hash|full_name|phone|role|auto_generated_pw`
- **Khi đọc:**  
  - Đọc từng trường để xác thực, hiển thị thông tin, kiểm tra trùng, phân quyền.

#### b) wallets.txt

- **Khi ghi:**  
  - Mỗi dòng:  
    `wallet_id|username|balance`
- **Khi đọc:**  
  - Đọc để xác định số dư, tìm ví theo username hoặc wallet_id.

#### c) transactions.txt

- **Khi ghi:**  
  - Mỗi dòng:  
    `from_wallet|to_wallet|amount|status|timestamp`
- **Khi đọc:**  
  - Hiển thị lịch sử giao dịch cho từng ví, trạng thái, thời gian (đã định dạng).

---

### 4. Input/Output xác thực OTP

- **Input:**  
  - Khi chuyển điểm, chương trình sinh mã OTP, gửi sang tiến trình otp_confirm.exe qua pipe.
  - Người dùng nhập OTP vào cửa sổ otp_confirm.exe để xác nhận.
- **Output:**  
  - Nếu OTP đúng: trả về "OK", giao dịch thành công.
  - Nếu OTP sai: trả về "CANCEL", giao dịch thất bại.

---

### 5. Input/Output cơ bản của C++ (cin/cout)

- **Input:**  
  - Nhập từ bàn phím qua `cin`, `getline(cin, ...)`
- **Output:**  
  - In ra màn hình qua `cout`, `endl`

---

### 6. Quy trình xử lý tổng quát

- Nhập dữ liệu → Kiểm tra hợp lệ → Ghi vào file (input)
- Đọc file → Xử lý, xác thực → In ra kết quả (output)
- Các thao tác quan trọng (chuyển điểm, đổi mật khẩu) đều yêu cầu xác thực lại qua OTP hoặc xác nhận mật khẩu.


## Nguồn tham khảo
- [1] https://github.com/okdshin/PicoSHA2/blob/master/picosha2.h
- [2] https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm
- [3] https://www.tutorialspoint.com/inter_process_communication/inter_process_communication_named_pipes.htm