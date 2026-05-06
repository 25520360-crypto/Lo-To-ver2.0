#include "include/TicketManager.h"
#include "include/Host.h"
#include "include/Player.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

using namespace std;

void printSeparator() { cout << "\n==================================================================" << endl; }
void printHeader(string title) {
    printSeparator();
    cout << "          " << title << endl;
    printSeparator();
}

int main() {
    TicketManager manager;
    Host host;
    Player player;

    long ticketPrice = 10000;
    long prizeMoney = 120000;
    int nSimulations;

    printHeader("CHUONG TRINH PHAN TICH CHIEN LUOC LO TO (16 VE)");

    cout << "[?] Nhap so lan muon mo phong: ";
    cin >> nSimulations;

    cout << "\n[*] He thong dang bat dau tinh toan..." << endl;
    manager.runGlobalSimulation(nSimulations);

    // IN KẾT QUẢ THỐNG KÊ VÉ ĐẦU TIÊN
    int bestId = host.pickBestTicket(manager); // Dùng Host để pick

    printHeader("BANG XEP HANG & VE VIP NHAT");
    cout << "=> KET QUA: To ve VIP nhat la #" << bestId << " (ti le ra som cao nhat)." << endl;
    cout << "   - Host nen luu y to #" << bestId << " vi no de khien song phai tra thuong nhat." << endl;
    manager.showWinStatistics(nSimulations);

    // PHẦN TRỌNG TÂM: PHÂN TÍCH KINH TẾ CHUNG (HOST & PLAYER)
    printHeader("PHAN TICH KINH TE & KIEM THU CHIEN LUOC");

    int loopPlayer = 1;
    while (loopPlayer == 1) {
        cout << "\n[?] Nhap gia ve thuc te (VND)  : "; cin >> ticketPrice;
        cout << "[?] Nhap gia tri giai thuong (VND): "; cin >> prizeMoney;

        // 1. Phân tích góc nhìn của Host
        host.analyzeHouseEdge(ticketPrice, prizeMoney);

        // 2. Phân tích góc nhìn của Player
        player.analyzeEconomics(ticketPrice, prizeMoney, nSimulations, manager);

        // 3. Tìm bộ vé tối ưu cho Player
        int soVe;
        cout << "\n[?] Ban muon mua may to ve de he thong tim bo toi uu? ";
        cin >> soVe;
        player.findBestSet(manager, soVe, ticketPrice, prizeMoney, nSimulations);

        // Hỏi xem có muốn test kịch bản giá/giải thưởng khác không
        cout << "\n------------------------------------------------------------------";
        cout << "\n[?] Ban co muon thu test kich ban Gia/Giai thuong khac khong? (1: Co, 0: Tiep tuc Demo): ";
        cin >> loopPlayer;
        if (loopPlayer == 1) {
            cout << "\n[*] TIEN HANH THIET LAP LAI THONG SO...\n";
            printSeparator();
        }
    }

    // PHẦN CUỐI: DEMO VAN DAU LIVE
    printSeparator();
    int choice;
    cout << "Ban co muon xem mot van dau LIVE de kiem chung khong? (1: Co, 0: Thoat): ";
    cin >> choice;

    if (choice == 1) {
        printHeader("DEMO VAN DAU LIVE - PHAN TICH TRUC TIEP");

        int maxTickets = manager.getPool().size();
        int guestId;

        // CHO PHÉP NGƯỜI CHƠI CHỌN VÉ
        cout << "\n[?] He thong co " << maxTickets << " to ve. Ban muon chon to nao de dau voi May tinh? (Nhap 1-" << maxTickets << "): ";
        while (true) {
            cin >> guestId;
            if (guestId >= 1 && guestId <= maxTickets) break;
            cout << "[!] ID khong hop le. Vui long nhap lai (1-" << maxTickets << "): ";
        }

        int botId = bestId;
        // Tránh trùng vé giữa Người và Máy
        if (botId == guestId) {
            botId = (bestId == 1) ? 2 : 1;
            cout << "\n[*] Vi ban da chon to VIP, May tinh se dung to #" << botId << " de thi dau!" << endl;
        }

        cin.ignore();
        cout << "\nNhan Enter de bat dau boc so..."; cin.get();

        LotoTicket& botVe = manager.getTicket(botId);
        LotoTicket& guestVe = manager.getTicket(guestId);

        cout << "\n[Nguoi choi 2] Giu ve : #" << botVe.getId() << " (May tinh chon)" << endl;
        cout << "[Ban]          Giu ve : #" << guestVe.getId() << endl;

        vector<int> longCau;
        for(int i=1; i<=90; i++) longCau.push_back(i);

        // Sử dụng random device chuẩn
        random_device rd;
        mt19937 g(rd());
        shuffle(longCau.begin(), longCau.end(), g);

        botVe.reset(); guestVe.reset();
        bool finish = false; int turn = 0;

        while(!finish && turn < 90) {
            int soRa = longCau[turn++];
            cout << "\n------------------------------------------" << endl;
            cout << "LUOT " << turn << " -> [ CON SO " << soRa << " ]" << endl;

            bool bTrung = botVe.checkNumber(soRa);
            bool gTrung = guestVe.checkNumber(soRa);

            if(bTrung) { cout << "[!] Nguoi choi 2 trung so " << soRa << endl; botVe.hienThi(); }
            if(gTrung) { cout << "[*] Ban trung so " << soRa << endl; guestVe.hienThi(); }

            if (!botVe.kiemTraKinh() && botVe.soSoConThieuDeKinh() == 1) cout << ">>> CANH BAO: Nguoi choi 2 chi con thieu 1 so!" << endl;
            if (!guestVe.kiemTraKinh() && guestVe.soSoConThieuDeKinh() == 1) cout << ">>> TIN VUI: Ban chi con thieu 1 so!" << endl;

            if(botVe.kiemTraKinh() || guestVe.kiemTraKinh()) {
                cout << "\n==========================================" << endl;
                if(botVe.kiemTraKinh()) cout << "   NGUOI CHOI 2 DA KINH ROI! (Ve #" << botVe.getId() << ")" << endl;
                if(guestVe.kiemTraKinh()) cout << "   BAN DA KINH ROI! (Ve #" << guestVe.getId() << ")" << endl;
                cout << "==========================================" << endl;
                finish = true;
            } else {
                if(bTrung || gTrung) {
                    cout << "Nhan Enter de tiep tuc..."; cin.get();
                }
            }
        }
    }

    printHeader("KET THUC GIA LAP");
    return 0;
}
