#include "XianHuanZhi.h"

// 后台管理类实现
void AdminManager::run() {
    showAdminMenu();
}

void AdminManager::showAdminMenu() {
    while (true) {
        clearScreen();
        cout << "==========================================" << endl;
        cout << "           闲置换后台管理系统           " << endl;
        cout << "==========================================" << endl;
        cout << "1. 用户管理" << endl;
        cout << "2. 商品管理" << endl;
        cout << "3. 系统统计" << endl;
        cout << "4. 返回主菜单" << endl;
        cout << "==========================================" << endl;
        cout << "请选择操作 (1-4): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1:
                manageUsers();
                break;
            case 2:
                manageItems();
                break;
            case 3:
                viewStatistics();
                break;
            case 4:
                return;
            default:
                cout << "无效的选择，请重新输入！" << endl;
                pressAnyKey();
                break;
        }
    }
}

void AdminManager::manageUsers() {
    while (true) {
        clearScreen();
        cout << "========== 用户管理 ==========" << endl;
        
        vector<User>& users = userManager.getAllUsers();
        
        // 显示用户列表
        cout << "\n用户列表：" << endl;
        cout << "==================================================================" << endl;
        cout << left << setw(15) << "用户名" << setw(20) << "邮箱" 
             << setw(12) << "角色" << setw(8) << "状态" << setw(15) << "注册时间" << endl;
        cout << "==================================================================" << endl;
        
        for (const auto& user : users) {
            string roleStr = (user.role == UserRole::ADMIN) ? "管理员" : "普通用户";
            string statusStr = user.isActive ? "活跃" : "停用";
            char timeStr[20];
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", localtime(&user.registerTime));
            
            cout << left << setw(15) << user.username
                 << setw(20) << (user.email.length() > 18 ? user.email.substr(0, 15) + "..." : user.email)
                 << setw(12) << roleStr
                 << setw(8) << statusStr
                 << setw(15) << timeStr << endl;
        }
        cout << "==================================================================" << endl;
        
        // 管理选项
        cout << "\n管理选项：" << endl;
        cout << "1. 查看用户详情" << endl;
        cout << "2. 删除用户" << endl;
        cout << "3. 修改用户角色" << endl;
        cout << "4. 停用/激活用户" << endl;
        cout << "5. 返回上级菜单" << endl;
        cout << "请选择操作 (1-5): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                string username;
                cout << "请输入用户名: ";
                cin >> username;
                showUserDetails(username);
                break;
            }
            case 2: {
                string username;
                cout << "请输入要删除的用户名: ";
                cin >> username;
                userManager.deleteUser(username);
                pressAnyKey();
                break;
            }
            case 3: {
                string username;
                int roleChoice;
                cout << "请输入用户名: ";
                cin >> username;
                cout << "选择角色 (1-普通用户, 2-管理员): ";
                cin >> roleChoice;
                
                UserRole newRole = (roleChoice == 2) ? UserRole::ADMIN : UserRole::NORMAL;
                userManager.updateUserRole(username, newRole);
                pressAnyKey();
                break;
            }
            case 4: {
                string username;
                int action;
                cout << "请输入用户名: ";
                cin >> username;
                cout << "选择操作 (1-停用, 2-激活): ";
                cin >> action;
                
                if (action == 1) {
                    userManager.deactivateUser(username);
                } else if (action == 2) {
                    userManager.activateUser(username);
                } else {
                    cout << "无效操作！" << endl;
                }
                pressAnyKey();
                break;
            }
            case 5:
                return;
            default:
                cout << "无效选择！" << endl;
                pressAnyKey();
                break;
        }
    }
}

void AdminManager::manageItems() {
    while (true) {
        clearScreen();
        cout << "========== 商品管理 ==========" << endl;
        
        // 使用公有方法显示所有商品
        itemManager.displayAllItemsForAdmin();
        
        // 管理选项
        cout << "\n管理选项：" << endl;
        cout << "1. 删除商品" << endl;
        cout << "2. 修改商品状态" << endl;
        cout << "3. 查看用户商品" << endl;
        cout << "4. 返回上级菜单" << endl;
        cout << "请选择操作 (1-4): ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int itemId;
                cout << "请输入要删除的商品ID: ";
                cin >> itemId;
                itemManager.removeItem(itemId, true);
                pressAnyKey();
                break;
            }
            case 2: {
                int itemId, statusChoice;
                cout << "请输入商品ID: ";
                cin >> itemId;
                cout << "选择状态 (1-可交易, 2-已售出, 3-已预订, 4-已下架): ";
                cin >> statusChoice;
                
                ItemStatus newStatus;
                switch (statusChoice) {
                    case 1: newStatus = ItemStatus::AVAILABLE; break;
                    case 2: newStatus = ItemStatus::SOLD; break;
                    case 3: newStatus = ItemStatus::RESERVED; break;
                    case 4: newStatus = ItemStatus::REMOVED; break;
                    default: 
                        cout << "无效状态！" << endl;
                        pressAnyKey();
                        continue;
                }
                
                itemManager.updateItemStatus(itemId, newStatus);
                pressAnyKey();
                break;
            }
            case 3: {
                string username;
                cout << "请输入用户名: ";
                cin >> username;
                // 使用公有方法显示用户商品
                itemManager.displayUserItemsForAdmin(username);
                break;
            }
            case 4:
                return;
            default:
                cout << "无效选择！" << endl;
                pressAnyKey();
                break;
        }
    }
}

void AdminManager::viewStatistics() {
    clearScreen();
    cout << "========== 系统统计 ==========" << endl;
    
    int totalUsers = userManager.getTotalUsersCount();
    int activeUsers = userManager.getActiveUsersCount();
    int totalItems = itemManager.getTotalItemsCount();
    int availableItems = itemManager.getAvailableItemsCount();
    int soldItems = itemManager.getSoldItemsCount();
    
    cout << "用户统计:" << endl;
    cout << "总用户数: " << totalUsers << endl;
    cout << "活跃用户: " << activeUsers << endl;
    cout << "停用用户: " << (totalUsers - activeUsers) << endl;
    
    cout << "\n商品统计:" << endl;
    cout << "总商品数: " << totalItems << endl;
    cout << "可交易商品: " << availableItems << endl;
    cout << "已售出商品: " << soldItems << endl;
    cout << "其他状态: " << (totalItems - availableItems - soldItems) << endl;
    
    cout << "\n系统信息:" << endl;
    cout << "当前时间: " << getCurrentTimeString() << endl;
    
    pressAnyKey();
}

void AdminManager::showUserDetails(const string& username) {
    User* user = userManager.getUserByUsername(username);
    if (!user) {
        cout << "未找到用户: " << username << endl;
        pressAnyKey();
        return;
    }
    
    clearScreen();
    cout << "========== 用户详情 ==========" << endl;
    cout << "用户名: " << user->username << endl;
    cout << "邮箱: " << user->email << endl;
    cout << "手机号: " << user->phone << endl;
    cout << "角色: " << (user->role == UserRole::ADMIN ? "管理员" : "普通用户") << endl;
    cout << "状态: " << (user->isActive ? "活跃" : "停用") << endl;
    
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&user->registerTime));
    cout << "注册时间: " << timeStr << endl;
    
    // 显示用户发布的商品
    vector<Item> userItems = itemManager.getItemsByUser(username);
    cout << "\n发布的商品数量: " << userItems.size() << endl;
    
    if (!userItems.empty()) {
        cout << "商品列表:" << endl;
        for (const auto& item : userItems) {
            // 使用公有方法获取状态字符串
            cout << "  ID: " << item.id << " - " << item.name 
                 << " (" << itemManager.getStatusString(item.status) << ")" << endl;
        }
    }
    
    pressAnyKey();
}