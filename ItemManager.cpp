#include "XianHuanZhi.h"

// 商品管理类实现
ItemManager::ItemManager() {
    loadItems();
}

ItemManager::~ItemManager() {
    saveItems();
}

void ItemManager::loadItems() {
    ifstream file(dataFile, ios::binary);
    if (!file) {
        return; // 文件不存在，首次运行
    }
    
    Item item;
    while (file.read(reinterpret_cast<char*>(&item), sizeof(Item))) {
        items.push_back(item);
        if (item.id >= nextItemId) {
            nextItemId = item.id + 1;
        }
    }
    file.close();
}

void ItemManager::saveItems() {
    ofstream file(dataFile, ios::binary | ios::trunc);
    if (!file) {
        cerr << "错误：无法保存商品数据！" << endl;
        return;
    }
    
    for (const auto& item : items) {
        file.write(reinterpret_cast<const char*>(&item), sizeof(Item));
    }
    file.close();
}

// 将这两个函数改为公有，或者提供公有访问方法
string ItemManager::categoryToString(Category category) {
    switch (category) {
        case Category::ELECTRONICS: return "电子产品";
        case Category::CLOTHING: return "服装鞋帽";
        case Category::BOOKS: return "图书文具";
        case Category::SPORTS: return "运动户外";
        case Category::DAILY_NEEDS: return "生活日用";
        case Category::OTHER: return "其他";
        default: return "未知";
    }
}

string ItemManager::statusToString(ItemStatus status) {
    switch (status) {
        case ItemStatus::AVAILABLE: return "可交易";
        case ItemStatus::SOLD: return "已售出";
        case ItemStatus::RESERVED: return "已预订";
        case ItemStatus::REMOVED: return "已下架";
        default: return "未知";
    }
}

Category ItemManager::selectCategory() {
    int choice;
    cout << "\n请选择商品类别：" << endl;
    cout << "1. 电子产品" << endl;
    cout << "2. 服装鞋帽" << endl;
    cout << "3. 图书文具" << endl;
    cout << "4. 运动户外" << endl;
    cout << "5. 生活日用" << endl;
    cout << "6. 其他" << endl;
    cout << "请选择 (1-6): ";
    
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 6) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "无效选择，请重新输入 (1-6): ";
        cin >> choice;
    }
    
    switch (choice) {
        case 1: return Category::ELECTRONICS;
        case 2: return Category::CLOTHING;
        case 3: return Category::BOOKS;
        case 4: return Category::SPORTS;
        case 5: return Category::DAILY_NEEDS;
        case 6: return Category::OTHER;
        default: return Category::OTHER;
    }
}

bool ItemManager::publishItem(const string& username) {
    clearScreen();
    cout << "========== 发布商品 ==========" << endl;
    
    Item newItem;
    newItem.id = nextItemId++;
    newItem.owner = username;
    newItem.publishTime = time(nullptr);
    newItem.status = ItemStatus::AVAILABLE;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // 输入商品名称
    while (true) {
        cout << "请输入商品名称: ";
        getline(cin, newItem.name);
        newItem.name = trim(newItem.name);
        
        if (newItem.name.empty()) {
            cout << "商品名称不能为空！" << endl;
        } else if (newItem.name.length() > 50) {
            cout << "商品名称不能超过50个字符！" << endl;
        } else {
            break;
        }
    }
    
    // 输入商品描述
    while (true) {
        cout << "请输入商品描述: ";
        getline(cin, newItem.description);
        newItem.description = trim(newItem.description);
        
        if (newItem.description.empty()) {
            cout << "商品描述不能为空！" << endl;
        } else if (newItem.description.length() > 200) {
            cout << "商品描述不能超过200个字符！" << endl;
        } else {
            break;
        }
    }
    
    // 输入价格
    while (true) {
        cout << "请输入价格: ";
        cin >> newItem.price;
        
        if (cin.fail() || newItem.price < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "价格必须是非负数字！" << endl;
        } else {
            break;
        }
    }
    
    // 选择类别
    newItem.category = selectCategory();
    
    // 输入联系方式
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << "请输入联系方式: ";
        getline(cin, newItem.contactInfo);
        newItem.contactInfo = trim(newItem.contactInfo);
        
        if (newItem.contactInfo.empty()) {
            cout << "联系方式不能为空！" << endl;
        } else {
            break;
        }
    }
    
    // 保存商品
    items.push_back(newItem);
    saveItems();
    
    cout << "\n商品发布成功！商品ID: " << newItem.id << endl;
    pressAnyKey();
    return true;
}

void ItemManager::searchItems() {
    clearScreen();
    cout << "========== 商品搜索 ==========" << endl;
    cout << "1. 关键词搜索" << endl;
    cout << "2. 分类搜索" << endl;
    cout << "3. 价格范围搜索" << endl;
    cout << "4. 显示所有商品" << endl;
    cout << "5. 返回上级菜单" << endl;
    cout << "请选择搜索方式 (1-5): ";
    
    int choice;
    cin >> choice;
    
    vector<Item> results;
    
    switch (choice) {
        case 1: {
            cin.ignore();
            string keyword;
            cout << "请输入关键词: ";
            getline(cin, keyword);
            keyword = trim(keyword);
            
            results = searchByKeyword(keyword);
            break;
        }
        case 2: {
            Category category = selectCategory();
            results = searchByCategory(category);
            break;
        }
        case 3: {
            double minPrice, maxPrice;
            cout << "请输入最低价格: ";
            cin >> minPrice;
            cout << "请输入最高价格: ";
            cin >> maxPrice;
            
            if (minPrice < 0 || maxPrice < minPrice) {
                cout << "价格范围无效！" << endl;
                pressAnyKey();
                return;
            }
            
            results = searchByPriceRange(minPrice, maxPrice);
            break;
        }
        case 4: {
            results = items; // 显示所有商品
            break;
        }
        case 5: {
            return;
        }
        default: {
            cout << "无效选择！" << endl;
            pressAnyKey();
            return;
        }
    }
    
    // 显示搜索结果
    displayItemsList(results);
}

void ItemManager::displayUserItems(const string& username) {
    clearScreen();
    cout << "========== 我发布的商品 ==========" << endl;
    
    vector<Item> userItems;
    for (const auto& item : items) {
        if (item.owner == username) {
            userItems.push_back(item);
        }
    }
    
    if (userItems.empty()) {
        cout << "您还没有发布任何商品。" << endl;
    } else {
        displayItemsList(userItems, true); // 显示用户的所有商品，包括已下架的
    }
    pressAnyKey();
}

void ItemManager::displayAllItems() {
    clearScreen();
    cout << "========== 所有商品 ==========" << endl;
    displayItemsList(items); // 只显示可交易的商品
}

// 私有辅助函数，用于显示商品列表
void ItemManager::displayItemsList(const vector<Item>& itemsToDisplay, bool showAll) {
    if (itemsToDisplay.empty()) {
        cout << "没有找到商品。" << endl;
        pressAnyKey();
        return;
    }
    
    vector<Item> displayItems;
    if (showAll) {
        // 显示所有商品，包括已下架的
        displayItems = itemsToDisplay;
    } else {
        // 只显示可交易的商品
        for (const auto& item : itemsToDisplay) {
            if (item.status == ItemStatus::AVAILABLE) {
                displayItems.push_back(item);
            }
        }
    }
    
    if (displayItems.empty()) {
        cout << "没有可显示的商品。" << endl;
        pressAnyKey();
        return;
    }
    
    cout << "\n商品列表：" << endl;
    cout << "==================================================================================" << endl;
    cout << left << setw(8) << "ID" << setw(20) << "商品名称" 
         << setw(12) << "价格" << setw(12) << "类别" 
         << setw(12) << "状态" << setw(15) << "发布者" << endl;
    cout << "==================================================================================" << endl;
    
    for (const auto& item : displayItems) {
        cout << left << setw(8) << item.id 
             << setw(20) << (item.name.length() > 18 ? item.name.substr(0, 15) + "..." : item.name)
             << setw(12) << fixed << setprecision(2) << item.price
             << setw(12) << categoryToString(item.category)
             << setw(12) << statusToString(item.status)
             << setw(15) << (item.owner.length() > 12 ? item.owner.substr(0, 9) + "..." : item.owner)
             << endl;
    }
    cout << "==================================================================================" << endl;
    
    // 提供查看详情的选项
    cout << "\n输入商品ID查看详情 (输入0返回): ";
    int itemId;
    cin >> itemId;
    
    if (itemId != 0) {
        displayItemDetails(itemId);
    }
}

void ItemManager::displayItemDetails(int itemId) {
    auto it = find_if(items.begin(), items.end(), 
        [itemId](const Item& item) { return item.id == itemId; });
    
    if (it == items.end()) {
        cout << "未找到该商品！" << endl;
        pressAnyKey();
        return;
    }
    
    clearScreen();
    cout << "========== 商品详情 ==========" << endl;
    cout << "商品ID: " << it->id << endl;
    cout << "商品名称: " << it->name << endl;
    cout << "商品描述: " << it->description << endl;
    cout << "价格: " << fixed << setprecision(2) << it->price << "元" << endl;
    cout << "类别: " << categoryToString(it->category) << endl;
    cout << "发布者: " << it->owner << endl;
    cout << "状态: " << statusToString(it->status) << endl;
    cout << "联系方式: " << it->contactInfo << endl;
    
    // 格式化时间显示
    char timeStr[20];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", localtime(&it->publishTime));
    cout << "发布时间: " << timeStr << endl;
    
    pressAnyKey();
}

vector<Item> ItemManager::searchByKeyword(const string& keyword) {
    vector<Item> results;
    string lowerKeyword = keyword;
    transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    for (const auto& item : items) {
        string lowerName = item.name;
        string lowerDesc = item.description;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        transform(lowerDesc.begin(), lowerDesc.end(), lowerDesc.begin(), ::tolower);
        
        if (lowerName.find(lowerKeyword) != string::npos || 
            lowerDesc.find(lowerKeyword) != string::npos) {
            results.push_back(item);
        }
    }
    return results;
}

vector<Item> ItemManager::searchByCategory(Category category) {
    vector<Item> results;
    for (const auto& item : items) {
        if (item.category == category) {
            results.push_back(item);
        }
    }
    return results;
}

vector<Item> ItemManager::searchByPriceRange(double minPrice, double maxPrice) {
    vector<Item> results;
    for (const auto& item : items) {
        if (item.price >= minPrice && item.price <= maxPrice) {
            results.push_back(item);
        }
    }
    return results;
}

bool ItemManager::removeItem(int itemId, bool isAdmin) {
    auto it = find_if(items.begin(), items.end(), 
        [itemId](const Item& item) { return item.id == itemId; });
    
    if (it != items.end()) {
        if (isAdmin) {
            // 管理员直接删除
            items.erase(it);
            cout << "商品 ID: " << itemId << " 已被管理员删除" << endl;
        } else {
            // 普通用户只能下架
            it->status = ItemStatus::REMOVED;
            cout << "商品 ID: " << itemId << " 已下架" << endl;
        }
        saveItems();
        return true;
    }
    
    cout << "未找到商品 ID: " << itemId << endl;
    return false;
}

bool ItemManager::updateItemStatus(int itemId, ItemStatus newStatus) {
    auto it = find_if(items.begin(), items.end(), 
        [itemId](const Item& item) { return item.id == itemId; });
    
    if (it != items.end()) {
        it->status = newStatus;
        saveItems();
        cout << "商品 ID: " << itemId << " 状态已更新为: " << statusToString(newStatus) << endl;
        return true;
    }
    
    cout << "未找到商品 ID: " << itemId << endl;
    return false;
}

int ItemManager::getAvailableItemsCount() const {
    return count_if(items.begin(), items.end(), 
        [](const Item& item) { return item.status == ItemStatus::AVAILABLE; });
}

int ItemManager::getSoldItemsCount() const {
    return count_if(items.begin(), items.end(), 
        [](const Item& item) { return item.status == ItemStatus::SOLD; });
}

vector<Item> ItemManager::getItemsByUser(const string& username) {
    vector<Item> userItems;
    for (const auto& item : items) {
        if (item.owner == username) {
            userItems.push_back(item);
        }
    }
    return userItems;
}

// 为后台管理提供公有访问方法
void ItemManager::displayAllItemsForAdmin() {
    displayItemsList(items, true);
}

void ItemManager::displayUserItemsForAdmin(const string& username) {
    vector<Item> userItems = getItemsByUser(username);
    displayItemsList(userItems, true);
}

string ItemManager::getStatusString(ItemStatus status) {
    return statusToString(status);
}