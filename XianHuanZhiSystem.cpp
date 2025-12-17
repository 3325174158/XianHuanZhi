#include "XianHuanZhi.h"

// 主系统类实现
void XianHuanZhiSystem::run() {
    int choice;
    
    do {
        if (userManager.isLoggedIn()) {
            if (userManager.getCurrentUserRole() == UserRole::ADMIN) {
                showAdminMenu();
            } else {
                showUserMenu();
            }
        } else {
            showMainMenu();
        }
    } while (true);
}

void XianHuanZhiSystem::showMainMenu() {
    clearScreen();
    cout << "==========================================" << endl;
    cout << "           欢迎使用闲置换软件            " << endl;
    cout << "==========================================" << endl;
    cout << "1. 用户注册" << endl;
    cout << "2. 用户登录" << endl;
    cout << "3. 浏览商品" << endl;
    cout << "4. 搜索商品" << endl;
    cout << "5. 退出系统" << endl;
    cout << "==========================================" << endl;
    cout << "请选择操作 (1-5): ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            userManager.registerUser();
            break;
        case 2:
            userManager.loginUser();
            break;
        case 3:
            itemManager.displayAllItems();
            break;
        case 4:
            itemManager.searchItems();
            break;
        case 5:
            clearScreen();
            cout << "感谢使用闲置换软件，再见！" << endl;
            exit(0);
        default:
            cout << "无效的选择，请重新输入！" << endl;
            pressAnyKey();
            break;
    }
}

void XianHuanZhiSystem::showUserMenu() {
    clearScreen();
    string username = userManager.getCurrentUsername();
    cout << "==========================================" << endl;
    cout << "     闲置换 - 欢迎 " << username << "!     " << endl;
    cout << "==========================================" << endl;
    cout << "1. 发布商品" << endl;
    cout << "2. 我的商品" << endl;
    cout << "3. 浏览商品" << endl;
    cout << "4. 搜索商品" << endl;
    cout << "5. 用户信息" << endl;
    cout << "6. 退出登录" << endl;
    cout << "7. 退出系统" << endl;
    cout << "==========================================" << endl;
    cout << "请选择操作 (1-7): ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            itemManager.publishItem(username);
            break;
        case 2:
            itemManager.displayUserItems(username);
            break;
        case 3:
            itemManager.displayAllItems();
            break;
        case 4:
            itemManager.searchItems();
            break;
        case 5:
            userManager.displayCurrentUser();
            break;
        case 6:
            userManager.logout();
            break;
        case 7:
            clearScreen();
            cout << "感谢使用闲置换软件，再见！" << endl;
            exit(0);
        default:
            cout << "无效的选择，请重新输入！" << endl;
            pressAnyKey();
            break;
    }
}

void XianHuanZhiSystem::showAdminMenu() {
    clearScreen();
    string username = userManager.getCurrentUsername();
    cout << "==========================================" << endl;
    cout << "   闲置换后台管理 - 欢迎 " << username << "!   " << endl;
    cout << "==========================================" << endl;
    cout << "1. 进入后台管理" << endl;
    cout << "2. 进入用户界面" << endl;
    cout << "3. 退出登录" << endl;
    cout << "4. 退出系统" << endl;
    cout << "==========================================" << endl;
    cout << "请选择操作 (1-4): ";
    
    int choice;
    cin >> choice;
    
    switch (choice) {
        case 1:
            adminManager.run();
            break;
        case 2:
            showUserMenu();
            break;
        case 3:
            userManager.logout();
            break;
        case 4:
            clearScreen();
            cout << "感谢使用闲置换软件，再见！" << endl;
            exit(0);
        default:
            cout << "无效的选择，请重新输入！" << endl;
            pressAnyKey();
            break;
    }
}