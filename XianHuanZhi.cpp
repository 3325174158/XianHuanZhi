#include "XianHuanZhi.h"
#include <cstdlib>  // 用于 system() 函数

// 用户管理类实现
UserManager::UserManager() : currentUser(nullptr) {
    loadUsers();
    
    // 如果没有用户，创建默认管理员账户
    if (users.empty()) {
        User admin;
        admin.username = "admin";
        admin.password = encryptPassword("admin123");
        admin.email = "admin@xianhuanzhi.com";
        admin.phone = "13800000000";
        admin.role = UserRole::ADMIN;
        admin.registerTime = time(nullptr);
        admin.isActive = true;
        
        users.push_back(admin);
        saveUsers();
        cout << "已创建默认管理员账户: admin / admin123" << endl;
    }
}

UserManager::~UserManager() {
    saveUsers();
}

void UserManager::loadUsers() {
    ifstream file(dataFile, ios::binary);
    if (!file) {
        return; // 文件不存在，首次运行
    }
    
    User user;
    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        users.push_back(user);
    }
    file.close();
}

void UserManager::saveUsers() {
    ofstream file(dataFile, ios::binary | ios::trunc);
    if (!file) {
        cerr << "错误：无法保存用户数据！" << endl;
        return;
    }
    
    for (const auto& user : users) {
        file.write(reinterpret_cast<const char*>(&user), sizeof(User));
    }
    file.close();
}

bool UserManager::isUsernameExist(const string& username) {
    return any_of(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
}

bool UserManager::isEmailExist(const string& email) {
    return any_of(users.begin(), users.end(), 
        [&email](const User& user) { return user.email == email; });
}

// 简单的密码加密（实际项目中应该使用更安全的加密方式）
string UserManager::encryptPassword(const string& password) {
    string encrypted = password;
    for (char& c : encrypted) {
        c = c + 1; // 简单的字符偏移加密
    }
    return encrypted;
}

bool UserManager::registerUser() {
    clearScreen();
    cout << "========== 闲置换用户注册 ==========" << endl;
    
    User newUser;
    newUser.role = UserRole::NORMAL;
    newUser.registerTime = time(nullptr);
    newUser.isActive = true;
    
    // 输入用户名
    while (true) {
        cout << "请输入用户名 (3-20个字符): ";
        cin >> newUser.username;
        
        if (newUser.username.length() < 3 || newUser.username.length() > 20) {
            cout << "用户名长度必须在3-20个字符之间！" << endl;
            continue;
        }
        
        if (isUsernameExist(newUser.username)) {
            cout << "用户名已存在，请选择其他用户名！" << endl;
        } else {
            break;
        }
    }
    
    // 输入密码
    while (true) {
        cout << "请输入密码 (6-20个字符): ";
        cin >> newUser.password;
        
        if (newUser.password.length() < 6 || newUser.password.length() > 20) {
            cout << "密码长度必须在6-20个字符之间！" << endl;
        } else {
            break;
        }
    }
    
    // 确认密码
    string confirmPassword;
    cout << "请再次输入密码: ";
    cin >> confirmPassword;
    
    if (newUser.password != confirmPassword) {
        cout << "两次输入的密码不一致！" << endl;
        pressAnyKey();
        return false;
    }
    
    // 加密密码
    newUser.password = encryptPassword(newUser.password);
    
    // 输入邮箱
    while (true) {
        cout << "请输入邮箱: ";
        cin >> newUser.email;
        
        if (!isValidEmail(newUser.email)) {
            cout << "邮箱格式不正确！" << endl;
            continue;
        }
        
        if (isEmailExist(newUser.email)) {
            cout << "该邮箱已被注册！" << endl;
        } else {
            break;
        }
    }
    
    // 输入手机号
    while (true) {
        cout << "请输入手机号: ";
        cin >> newUser.phone;
        
        if (!isValidPhone(newUser.phone)) {
            cout << "手机号格式不正确！" << endl;
        } else {
            break;
        }
    }
    
    // 保存用户
    users.push_back(newUser);
    saveUsers();
    
    cout << "\n注册成功！欢迎加入闲置换！" << endl;
    pressAnyKey();
    return true;
}

bool UserManager::loginUser() {
    clearScreen();
    cout << "========== 闲置换用户登录 ==========" << endl;
    
    if (isLoggedIn()) {
        cout << "您已经登录了！" << endl;
        pressAnyKey();
        return true;
    }
    
    string username, password;
    
    cout << "请输入用户名: ";
    cin >> username;
    
    cout << "请输入密码: ";
    cin >> password;
    
    // 加密输入的密码进行比较
    string encryptedPassword = encryptPassword(password);
    
    // 查找用户
    auto it = find_if(users.begin(), users.end(), 
        [&username, &encryptedPassword](const User& user) {
            return user.username == username && user.password == encryptedPassword && user.isActive;
        });
    
    if (it != users.end()) {
        currentUser = &(*it);
        cout << "\n登录成功！欢迎回来，" << username << "！" << endl;
        if (currentUser->role == UserRole::ADMIN) {
            cout << "您以管理员身份登录。" << endl;
        }
        pressAnyKey();
        return true;
    } else {
        // 检查是否是账户被停用
        auto userIt = find_if(users.begin(), users.end(), 
            [&username](const User& user) { return user.username == username; });
        
        if (userIt != users.end() && !userIt->isActive) {
            cout << "\n账户已被停用，请联系管理员！" << endl;
        } else {
            cout << "\n用户名或密码错误！" << endl;
        }
        pressAnyKey();
        return false;
    }
}

void UserManager::logout() {
    if (currentUser) {
        cout << "用户 " << currentUser->username << " 已退出登录！" << endl;
        currentUser = nullptr;
    } else {
        cout << "当前没有用户登录！" << endl;
    }
    pressAnyKey();
}

void UserManager::displayCurrentUser() {
    clearScreen();
    cout << "========== 当前用户信息 ==========" << endl;
    
    if (isLoggedIn()) {
        cout << "用户名: " << currentUser->username << endl;
        cout << "邮箱: " << currentUser->email << endl;
        cout << "手机号: " << currentUser->phone << endl;
        cout << "角色: " << (currentUser->role == UserRole::ADMIN ? "管理员" : "普通用户") << endl;
        cout << "状态: " << (currentUser->isActive ? "活跃" : "停用") << endl;
        
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&currentUser->registerTime));
        cout << "注册时间: " << timeStr << endl;
    } else {
        cout << "当前没有用户登录！" << endl;
    }
    pressAnyKey();
}

bool UserManager::isLoggedIn() {
    return currentUser != nullptr;
}

// 后台管理功能实现
bool UserManager::deleteUser(const string& username) {
    if (username == "admin") {
        cout << "不能删除管理员账户！" << endl;
        return false;
    }
    
    auto it = find_if(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
    
    if (it != users.end()) {
        users.erase(it);
        saveUsers();
        cout << "用户 " << username << " 已被删除" << endl;
        return true;
    }
    
    cout << "未找到用户: " << username << endl;
    return false;
}

bool UserManager::updateUserRole(const string& username, UserRole newRole) {
    auto it = find_if(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
    
    if (it != users.end()) {
        it->role = newRole;
        saveUsers();
        cout << "用户 " << username << " 角色已更新" << endl;
        return true;
    }
    
    cout << "未找到用户: " << username << endl;
    return false;
}

bool UserManager::deactivateUser(const string& username) {
    if (username == "admin") {
        cout << "不能停用管理员账户！" << endl;
        return false;
    }
    
    auto it = find_if(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
    
    if (it != users.end()) {
        it->isActive = false;
        saveUsers();
        cout << "用户 " << username << " 已被停用" << endl;
        return true;
    }
    
    cout << "未找到用户: " << username << endl;
    return false;
}

bool UserManager::activateUser(const string& username) {
    auto it = find_if(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
    
    if (it != users.end()) {
        it->isActive = true;
        saveUsers();
        cout << "用户 " << username << " 已被激活" << endl;
        return true;
    }
    
    cout << "未找到用户: " << username << endl;
    return false;
}

int UserManager::getActiveUsersCount() const {
    return count_if(users.begin(), users.end(), 
        [](const User& user) { return user.isActive; });
}

User* UserManager::getUserByUsername(const string& username) {
    auto it = find_if(users.begin(), users.end(), 
        [&username](const User& user) { return user.username == username; });
    
    return (it != users.end()) ? &(*it) : nullptr;
}

// 工具函数实现
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pressAnyKey() {
    cout << "\n按任意键继续..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool isValidEmail(const string& email) {
    // 简单的邮箱验证
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    
    return (atPos != string::npos && 
            dotPos != string::npos && 
            dotPos > atPos + 1 && 
            email.length() > dotPos + 1);
}

bool isValidPhone(const string& phone) {
    // 简单的手机号验证（11位数字）
    if (phone.length() != 11) {
        return false;
    }
    
    return all_of(phone.begin(), phone.end(), ::isdigit);
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    
    if (start == string::npos) {
        return "";
    }
    
    return str.substr(start, end - start + 1);
}

// 获取当前时间字符串
string getCurrentTimeString() {
    time_t now = time(nullptr);
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(timeStr);
}