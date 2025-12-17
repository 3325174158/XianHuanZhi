#ifndef XIANHUANZHI_H
#define XIANHUANZHI_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <iomanip>

using namespace std;

// 用户角色枚举
enum class UserRole {
    NORMAL,     // 普通用户
    ADMIN       // 管理员
};

// 商品类别枚举
enum class Category {
    ELECTRONICS,    // 电子产品
    CLOTHING,       // 服装鞋帽
    BOOKS,          // 图书文具
    SPORTS,         // 运动户外
    DAILY_NEEDS,    // 生活日用
    OTHER           // 其他
};

// 商品状态枚举
enum class ItemStatus {
    AVAILABLE,      // 可交易
    SOLD,           // 已售出
    RESERVED,       // 已预订
    REMOVED         // 已下架
};

// 商品数据结构
struct Item {
    int id;                     // 商品ID
    string name;                // 商品名称
    string description;         // 商品描述
    double price;               // 价格
    Category category;          // 商品类别
    string owner;               // 发布者用户名
    time_t publishTime;         // 发布时间
    ItemStatus status;          // 商品状态
    string contactInfo;         // 联系方式
};

// 用户数据结构
struct User {
    string username;
    string password;
    string email;
    string phone;
    UserRole role;              // 用户角色
    time_t registerTime;        // 注册时间
    bool isActive;              // 账户状态
    vector<int> publishedItems; // 发布的商品ID列表
};

// 系统统计数据结构
struct SystemStats {
    int totalUsers;
    int totalItems;
    int activeUsers;
    int availableItems;
    int soldItems;
};

// 商品管理类
class ItemManager {
private:
    vector<Item> items;
    string dataFile = "items.dat";
    int nextItemId = 1;
    
    void loadItems();
    void saveItems();

    Category selectCategory();
    void displayItemsList(const vector<Item>& itemsToDisplay, bool showAll = false);
    
public:
    ItemManager();
    ~ItemManager();
        string categoryToString(Category category);
    string statusToString(ItemStatus status);
    bool publishItem(const string& username);
    void searchItems();
    void displayUserItems(const string& username);
    void displayAllItems();
    void displayItemDetails(int itemId);
    vector<Item> searchByKeyword(const string& keyword);
    vector<Item> searchByCategory(Category category);
    vector<Item> searchByPriceRange(double minPrice, double maxPrice);
    
    // 后台管理功能
    vector<Item>& getAllItems() { return items; }
    bool removeItem(int itemId, bool isAdmin = false);
    bool updateItemStatus(int itemId, ItemStatus newStatus);
    int getTotalItemsCount() const { return items.size(); }
    int getAvailableItemsCount() const;
    int getSoldItemsCount() const;
    vector<Item> getItemsByUser(const string& username);
    
    // 为后台管理提供公有访问方法
    void displayAllItemsForAdmin();
    void displayUserItemsForAdmin(const string& username);
    string getStatusString(ItemStatus status);
};

// 用户管理类
class UserManager {
private:
    vector<User> users;
    string dataFile = "users.dat";
    User* currentUser; // 当前登录用户
    
    void loadUsers();
    void saveUsers();
    bool isUsernameExist(const string& username);
    bool isEmailExist(const string& email);
    string encryptPassword(const string& password);
    
public:
    UserManager();
    ~UserManager();
    
    bool registerUser();
    bool loginUser();
    void logout();
    void displayCurrentUser();
    bool isLoggedIn();
    User* getCurrentUser() { return currentUser; }
    string getCurrentUsername() { return currentUser ? currentUser->username : ""; }
    UserRole getCurrentUserRole() { return currentUser ? currentUser->role : UserRole::NORMAL; }
    
    // 后台管理功能
    vector<User>& getAllUsers() { return users; }
    bool deleteUser(const string& username);
    bool updateUserRole(const string& username, UserRole newRole);
    bool deactivateUser(const string& username);
    bool activateUser(const string& username);
    int getTotalUsersCount() const { return users.size(); }
    int getActiveUsersCount() const;
    User* getUserByUsername(const string& username);
};

// 后台管理类
class AdminManager {
private:
    UserManager& userManager;
    ItemManager& itemManager;
    
    void showAdminMenu();
    void manageUsers();
    void manageItems();
    void viewStatistics();
    void showUserDetails(const string& username);
    
public:
    AdminManager(UserManager& um, ItemManager& im) : userManager(um), itemManager(im) {}
    void run();
};

// 主系统类
class XianHuanZhiSystem {
private:
    UserManager userManager;
    ItemManager itemManager;
    AdminManager adminManager;
    
    void showMainMenu();
    void showUserMenu();
    void showAdminMenu();
    
public:
    XianHuanZhiSystem() : adminManager(userManager, itemManager) {}
    void run();
};

// 工具函数
void clearScreen();
void pressAnyKey();
bool isValidEmail(const string& email);
bool isValidPhone(const string& phone);
string trim(const string& str);
string getCurrentTimeString();

#endif