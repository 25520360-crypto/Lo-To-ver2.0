# MiniProject-Group16-LoTo

## Thành viên trong nhóm
*   Lê Thanh Sang | MSSV: 25521576
*   Dương Quảng Phú | MSSV: 25521395
*   Lê Ngọc Dũng | MSSV: 25520360
*   Nguyễn Mạnh Quân | MSSV: 25521500

## I. Giới thiệu chung
Dự án này là một hệ thống mô phỏng, thống kê và phân tích chiến thuật cho trò chơi dân gian phổ biến trong dịp Tết ở Việt Nam: **Lô Tô (Chuẩn bộ 16 vé Tân Tân)**.

Dự án không chỉ nhằm mục đích tạo ra một trò chơi giải trí, mà còn là một công cụ nghiên cứu xác suất thống kê. Thông qua việc giả lập hàng trăm ngàn ván bài, hệ thống ghi nhận dữ liệu, tự động phân tích Expected Value (EV), House Edge, và đưa ra chiến thuật tối ưu cho cả Nhà cái (Host) và Người chơi (Player).

## II. Tính năng nổi bật

### 1. Trải nghiệm Lô Tô Thực Tế
*   **Chế độ xem 1 ván bình thường:** Mô phỏng chân thực một ván loto với toàn bộ 16 tờ vé tham gia.
*   **Bốc từng số:** Hệ thống bốc từng số ngẫu nhiên, tự động rà soát 16 vé và in chi tiết tờ vé ra màn hình nếu có vé trúng số đó.
*   **Cảnh báo & Dừng:** Tự động cảnh báo khi có vé "chờ Kinh" (chỉ còn thiếu 1 số) và kết thúc ván ngay lập tức khi phát hiện vé đã "Kinh".

### 2. Core Engine & Thống kê
*   Quản lý bộ 16 tờ vé Lô tô chuẩn xác (dữ liệu thực tế từ bộ Tân Tân).
*   Hệ thống quay số ngẫu nhiên (sử dụng thư viện `<random>` và `mt19937` để đảm bảo tính phân tán cao và minh bạch).
*   Mô phỏng siêu tốc hàng trăm ngàn ván đấu chỉ trong vài giây để lấy mẫu thống kê tỉ lệ thắng (Win Rate) thực tế của từng tờ vé.

### 3. Phân tích chiến lược Nhà Cái (Host)
*   **Tính toán House Edge:** Dựa vào giá vé và giải thưởng, hệ thống tự động phân tích tỉ suất lợi nhuận của nhà cái theo công thức `Edge(%) = (16×giá_vé − giải_thưởng) / (16×giá_vé) × 100`.
*   **Cảnh báo rủi ro:** Đưa ra lời khuyên nếu giải thưởng quá cao (edge âm — nguy cơ lỗ) hoặc lợi nhuận quá lớn (edge > 20% — nguy cơ mất khách).
*   **Xác định Vé rủi ro cao nhất:** Phân tích dữ liệu mô phỏng để chỉ ra tờ vé có xác suất "Kinh" (thắng) sớm nhất — đây là tờ vé nhà cái cần lưu ý vì nó dễ dẫn đến việc phải chi trả giải thưởng nhất. 

### 4. Phân tích chiến lược Người Chơi (Player)
*   **Tính toán Expected Value (EV):** Đánh giá chính xác kỳ vọng lợi nhuận thuần dựa trên xác suất thắng và chi phí vốn.
*   **Thuật toán Tham Lam (Greedy Algorithm):** Tự động tìm ra bộ combo N tờ vé tối ưu nhất (độ phủ số rộng nhất, ít trùng lặp nhất) để tối đa hóa cơ hội thắng.
*   **Mô phỏng kiểm chứng chiến thuật (Monte Carlo):** So sánh trực quan giữa 2 chiến thuật kinh điển:
    *   *Single Play* (Chơi 1 ván).
    *   *Hit and Run* (Chơi đến khi thắng thì nghỉ).
    Hệ thống sẽ chạy thử với 100,000 người chơi ảo để chứng minh bằng Toán học: "Hit and Run sẽ làm người chơi lỗ nặng hơn rất nhiều khi EV đang âm".

### 5. Chế độ Demo Live (Bot vs Player)
*   Cho phép người chơi tự chọn 1 tờ vé bất kỳ (nhập ID từ 1–16) để đối đầu trực tiếp 1v1 với Máy tính (được trang bị tờ vé VIP nhất).
*   Hiển thị quá trình bốc số từng lượt một cách hồi hộp, báo hiệu người chơi / máy tính khi chỉ còn thiếu 1 số.

## III. Cấu trúc Mã Nguồn (OOP)
Dự án được thiết kế chặt chẽ theo mô hình Hướng Đối Tượng (Object-Oriented Programming):
*   `LotoTicket.cpp/.h`: Khai báo và quản lý thực thể Tờ Vé. Xử lý logic đánh dấu số, kiểm tra chiến thắng (Kinh), đếm số còn thiếu và in vé ra màn hình.
*   `TicketManager.cpp/.h`: Trung tâm dữ liệu. Khởi tạo pool 16 tờ vé, chịu trách nhiệm quay số, quản lý vòng đời mô phỏng và thu thập số liệu thống kê.
*   `Host.cpp/.h`: Xử lý logic và chiến thuật của Nhà cái (Tính House Edge, xác định tờ vé VIP).
*   `Player.cpp/.h`: Xử lý logic và chiến thuật của Người chơi (Tính EV, Thuật toán Greedy tìm bộ vé, chạy mô phỏng Hit & Run).
*   `main.cpp`: Khung xương điều hướng chương trình (Menu Chính), thiết lập vòng lặp giao diện người dùng và vận hành các chế độ Demo.

---
*Slide thuyết trình: https://canva.link/0y4t39xybf50nij*
