#include "../include/Player.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random> // THÊM THƯ VIỆN NÀY ĐỂ CHẠY MÔ PHỎNG NGẪU NHIÊN

Player::Player() {}

// ... (Giữ nguyên hàm analyzeEconomics) ...
void Player::analyzeEconomics(long giaVe, long giaiThuong, int totalGamesSimulation, const TicketManager& manager) {
    auto pool = manager.getPool();
    std::cout << "\n--- PHAN TICH CHIEN LUOC KINH TE (Dua tren mo phong) ---" << std::endl;
    std::cout << "Gia ve: " << giaVe << " VND | Giai thuong: " << giaiThuong << " VND" << std::endl;

    int bestIdx = 0, worstIdx = 0;
    for (size_t i = 1; i < pool.size(); ++i) {
        if (pool[i].getWinCount() > pool[bestIdx].getWinCount()) bestIdx = i;
        if (pool[i].getWinCount() < pool[worstIdx].getWinCount()) worstIdx = i;
    }

    double pBest = (double)pool[bestIdx].getWinCount() / totalGamesSimulation;
    double pWorst = (double)pool[worstIdx].getWinCount() / totalGamesSimulation;

    double evBest = (pBest * giaiThuong) - giaVe;
    double evWorst = (pWorst * giaiThuong) - giaVe;

    std::cout << "1. Mua to VIP (#" << pool[bestIdx].getId() << "): EV = " << evBest << " VND/van" << std::endl;
    std::cout << "2. Mua to te nhat (#" << pool[worstIdx].getId() << "): EV = " << evWorst << " VND/van" << std::endl;

    if (evBest > 0) std::cout << "=> KET LUAN: Ban CO THE KIEM LOI neu chon duoc ve tot!" << std::endl;
    else std::cout << "=> KET LUAN: Ky vong am. NHA CAI LUON THANG, KHONG NEN CHOI!" << std::endl;
}

// HÀM FINDBESSET MỚI ĐÃ TÍCH HỢP MÔ PHỎNG CHIẾN THUẬT
void Player::findBestSet(const TicketManager& manager, int soLuongVe, long giaVe, long giaiThuong, int totalGamesSimulation) {
    auto pool = manager.getPool();
    if (soLuongVe <= 0 || soLuongVe > (int)pool.size()) {
        std::cout << "[!] So luong ve khong hop le!" << std::endl;
        return;
    }

    std::vector<int> selectedTickets;
    std::set<int> coveredNumbers;

    // Tìm vé gốc
    int bestIdx = 0;
    for (size_t i = 1; i < pool.size(); ++i) {
        if (pool[i].getWinCount() > pool[bestIdx].getWinCount()) bestIdx = i;
    }

    selectedTickets.push_back(bestIdx);
    coveredNumbers.insert(pool[bestIdx].getDanhSachSo().begin(), pool[bestIdx].getDanhSachSo().end());

    // Thuật toán Greedy
    for (int step = 1; step < soLuongVe; ++step) {
        int bestNextIdx = -1;
        int maxNewNumbers = -1;

        for (size_t i = 0; i < pool.size(); ++i) {
            if (std::find(selectedTickets.begin(), selectedTickets.end(), i) != selectedTickets.end()) continue;
            int newNumbersCount = 0;
            for (int so : pool[i].getDanhSachSo()) {
                if (coveredNumbers.find(so) == coveredNumbers.end()) newNumbersCount++;
            }
            if (newNumbersCount > maxNewNumbers) {
                maxNewNumbers = newNumbersCount;
                bestNextIdx = i;
            }
        }
        selectedTickets.push_back(bestNextIdx);
        for (int so : pool[bestNextIdx].getDanhSachSo()) coveredNumbers.insert(so);
    }

    std::cout << "\n=> BO " << soLuongVe << " VE TOI UU NHAT (Thuat toan Tham Lam):" << std::endl;
    std::cout << "Gom cac ve ID: ";

    int totalWinCountOfSet = 0;
    for (int idx : selectedTickets) {
        std::cout << "#" << pool[idx].getId() << " ";
        totalWinCountOfSet += pool[idx].getWinCount();
    }
    std::cout << "\nTong do phu: " << coveredNumbers.size() << "/90 so." << std::endl;

    double pWinSet = (double)totalWinCountOfSet / totalGamesSimulation;
    long costPerGame = soLuongVe * giaVe;
    double expectedValue = (pWinSet * giaiThuong) - costPerGame;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "Xac suat thang cua bo ve  : " << pWinSet * 100 << "%" << std::endl;
    std::cout << "Loi nhuan ky vong (EV) bo : " << expectedValue << " VND/van" << std::endl;

    if (expectedValue > 0) {
        std::cout << "=> CHIEN LUOC: MUA BO NAY CO LAI (LOI THE THUOC VE BAN)!" << std::endl;
    } else {
        std::cout << "=> CHIEN LUOC: KHONG NEN MUA (SE BI LO VON VE LAU DAI)!" << std::endl;

        // IN LÝ THUYẾT CHIẾN THUẬT
        std::cout << "\n==================================================================" << std::endl;
        std::cout << " LUU Y KHI EV AM: CHIEN THUAT 'GIAM THIEU THUA LO'" << std::endl;
        std::cout << "==================================================================" << std::endl;
        std::cout << "\n1. Chien thuat 'Single Play' (Choi dung 1 van):" << std::endl;
        std::cout << "   - Vi EV am, choi cang nhieu se cang lo. Cach tot nhat la choi 1 lan." << std::endl;
        std::cout << "\n2. Chien thuat 'Hit and Run' (Thang la nghi):" << std::endl;
        std::cout << "   - Choi lien tuc toi da N van. Neu trung 1 lan thi dung luon." << std::endl;

        int n50 = (pWinSet > 0 && pWinSet < 1.0) ? std::ceil(std::log(0.5) / std::log(1.0 - pWinSet)) : 0;
        std::cout << "   - Voi ty le thang " << pWinSet * 100 << "%, ban can choi toi da " << n50 << " van de co 50% co hoi trung." << std::endl;

        // ĐOẠN MỚI: MÔ PHỎNG KIỂM CHỨNG LÝ THUYẾT
        int testSim;
        std::cout << "\n[?] Ban co muon mo phong 100,000 nguoi choi de kiem chung 2 chien thuat nay? (1: Co, 0: Khong): ";
        std::cin >> testSim;

        if (testSim == 1) {
            std::cout << "\n[*] DANG CHAY MO PHONG KIEM CHUNG..." << std::endl;
            int numPlayers = 100000;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);

            // 1. Mô phỏng Single Play
            int singleWins = 0;
            long long singleTotalProfit = 0;

            for (int i = 0; i < numPlayers; ++i) {
                bool isWin = (dis(gen) < pWinSet);
                if (isWin) {
                    singleWins++;
                    singleTotalProfit += (giaiThuong - costPerGame);
                } else {
                    singleTotalProfit -= costPerGame;
                }
            }

            // 2. Mô phỏng Hit and Run (Giới hạn tối đa n50 ván)
            int hitRunWins = 0;
            long long hitRunTotalProfit = 0;
            long long hitRunTotalGames = 0;

            for (int i = 0; i < numPlayers; ++i) {
                long long playerProfit = 0;
                bool won = false;
                for (int g = 1; g <= n50; ++g) {
                    playerProfit -= costPerGame;
                    hitRunTotalGames++;
                    if (dis(gen) < pWinSet) { // Trúng
                        playerProfit += giaiThuong;
                        won = true;
                        break; // Trúng là nghỉ ngay (Hit and Run)
                    }
                }
                if (won) hitRunWins++;
                hitRunTotalProfit += playerProfit;
            }

            // IN KẾT QUẢ MÔ PHỎNG
            std::cout << "\n--- KET QUA MO PHONG (Tong cong: 100,000 nguoi choi) ---" << std::endl;

            std::cout << "\n>> CHIEN THUAT 1: SINGLE PLAY (Moi nguoi chi mua dung 1 van)" << std::endl;
            std::cout << " - So van trung binh / nguoi : 1 van" << std::endl;
            std::cout << " - Ti le nguoi choi thang  : " << (double)singleWins / numPlayers * 100.0 << "%" << std::endl;
            std::cout << " - Lai/Lo trung binh       : " << singleTotalProfit / numPlayers << " VND/nguoi" << std::endl;

            std::cout << "\n>> CHIEN THUAT 2: HIT AND RUN (Choi toi da " << n50 << " van, thang la nghi)" << std::endl;
            std::cout << " - So van trung binh / nguoi : " << (double)hitRunTotalGames / numPlayers << " van" << std::endl;
            std::cout << " - Ti le nguoi choi thang  : " << (double)hitRunWins / numPlayers * 100.0 << "% (Thang it nhat 1 lan)" << std::endl;
            std::cout << " - Lai/Lo trung binh       : " << hitRunTotalProfit / numPlayers << " VND/nguoi" << std::endl;

            std::cout << "\n=> TONG KET TOAN HOC:" << std::endl;
            std::cout << "   Chien thuat 'Hit and Run' giup day xac suat ban cam tien thuong mang ve len rat cao." << std::endl;
            std::cout << "   NHUNG chinh vi EV dang am, ban cang ngoi lau thi tien mua ve (von) cang doi len." << std::endl;
            std::cout << "   Ket qua la: Hit and Run se lam ban LO TONG THE NANG HON Single Play rat nhieu!" << std::endl;
        }
    }
}
