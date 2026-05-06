# MiniProject-Group16-LoTo

## Thành viên trong nhóm
*   [Tên thành viên 1] | MSSV: [MSSV 1]
*   [Tên thành viên 2] | MSSV: [MSSV 2]
*   [Tên thành viên 3] | MSSV: [MSSV 3]
*   [Tên thành viên 4] | MSSV: [MSSV 4]
*   [Tên thành viên 5] | MSSV: [MSSV 5]

## I. Giới thiệu chung
Dự án này là một hệ thống mô phỏng, thống kê và phân tích chiến thuật cho trò chơi dân gian phổ biến trong dịp Tết ở Việt Nam: **Lô Tô (Chuẩn bộ 16 vé Tân Tân)**.

Dự án không chỉ nhằm mục đích tạo ra một trò chơi giải trí, mà còn là một công cụ nghiên cứu xác suất thống kê. Thông qua việc giả lập hàng trăm ngàn ván bài, hệ thống ghi nhận dữ liệu, tự động phân tích Expected Value (EV), House Edge, và đưa ra chiến thuật tối ưu cho cả Nhà cái (Host) và Người chơi (Player).

## II. Tính năng nổi bật

### 1. Core Engine
*   Quản lý bộ 16 tờ vé Lô tô chuẩn xác (dữ liệu thực tế từ bộ Tân Tân).
*   Hệ thống quay số ngẫu nhiên (sử dụng thư viện `<random>` và `mt19937` để đảm bảo tính phân tán cao và minh bạch).
*   Mô phỏng siêu tốc hàng trăm ngàn ván đấu chỉ trong vài giây để lấy mẫu thống kê độ chính xác cao.

### 2. Phân tích chiến lược Nhà Cái (Host)
*   **Tính toán House Edge:** Dựa vào giá vé và giải thưởng, hệ thống tự động phân tích tỉ suất lợi nhuận của nhà cái.
*   **Cảnh báo rủi ro:** Đưa ra lời khuyên nếu giải thưởng quá cao (nguy cơ lỗ) hoặc quá thấp (nguy cơ mất khách).
*   **Định vị Vé VIP:** Phân tích dữ liệu mô phỏng để chỉ ra tờ vé có xác suất "Kinh" (thắng) sớm nhất, giúp nhà cái tối ưu hóa chiến thuật (giữ vé lại hoặc đưa cho người nhà).

### 3. Phân tích chiến lược Người Chơi (Player)
*   **Tính toán Expected Value (EV):** Đánh giá chính xác kỳ vọng lợi nhuận thuần dựa trên xác suất thắng và chi phí vốn.
*   **Thuật toán Greedy (Tham Lam):** Tự động tìm ra bộ combo N tờ vé tối ưu nhất (độ phủ số rộng nhất, ít trùng lặp nhất) để tối đa hóa cơ hội thắng.
*   **Mô phỏng kiểm chứng chiến thuật (Monte Carlo):** So sánh trực quan giữa 2 chiến thuật kinh điển:
    *   *Single Play* (Chơi 1 ván).
    *   *Hit and Run* (Chơi đến khi thắng thì nghỉ).
    Hệ thống sẽ chạy thử với 100,000 người chơi ảo để chứng minh Toán học không bao giờ nói dối (Hit and Run sẽ làm người chơi lỗ nặng hơn khi EV âm).

### 4. Chế độ Demo Live (Tương tác trực tiếp)
*   Cho phép người chơi tự chọn 1 tờ vé bất kỳ để đối đầu trực tiếp 1v1 với Máy tính (được trang bị tờ vé VIP nhất).
*   Hiển thị quá trình bốc số từng lượt một cách hồi hộp, chân thực như đang chơi ngoài đời.
*   Tính năng cảnh báo thông minh: Thông báo ngay lập tức khi Người chơi hoặc Máy tính chỉ còn thiếu đúng 1 số nữa là "Kinh".

## III. Cấu trúc Mã Nguồn (OOP)
Dự án được thiết kế chặt chẽ theo mô hình Hướng Đối Tượng (Object-Oriented Programming):
*   `LotoTicket.cpp/.h`: Khai báo và quản lý thực thể Tờ Vé. Xử lý logic đánh dấu số, kiểm tra chiến thắng (Kinh), đếm số còn thiếu và in vé ra màn hình.
*   `TicketManager.cpp/.h`: Trung tâm dữ liệu. Khởi tạo pool 16 tờ vé, chịu trách nhiệm quay số, quản lý vòng đời mô phỏng và thu thập số liệu thống kê.
*   `Host.cpp/.h`: Xử lý logic và chiến thuật của Nhà cái (Tính House Edge, chọn vé VIP).
*   `Player.cpp/.h`: Xử lý logic và chiến thuật của Người chơi (Tính EV, Thuật toán Greedy tìm bộ vé, chạy mô phỏng chiến thuật Hit & Run).
*   `main.cpp`: Khung xương điều hướng chương trình, thiết lập vòng lặp giao diện người dùng và vận hành chế độ Demo Live.

---
*Slide thuyết trình: [Link Slide Của Nhóm]*