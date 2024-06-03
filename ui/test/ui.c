#include <ncurses.h>  // 包含ncurses库头文件
#include <string.h>   // 包含字符串操作的头文件

// 绘制窗口边框的函数
void draw_border(WINDOW *win) {
    box(win, 0, 0);   // 绘制窗口边框
    wrefresh(win);    // 刷新窗口以显示边框
}

// 显示登录界面的函数
void login_screen() {
    clear();   // 清屏
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);  // 获取终端的最大行列数

    // 创建一个新的窗口用于登录界面
    WINDOW *loginwin = newwin(10, 40, (yMax / 2) - 5, (xMax / 2) - 20);
    draw_border(loginwin);  // 绘制窗口边框

    // 在窗口中打印提示信息
    mvwprintw(loginwin, 1, 1, "Login");
    mvwprintw(loginwin, 3, 1, "Username: ");
    mvwprintw(loginwin, 5, 1, "Password: ");

    char username[30], password[30];
    // 获取用户名和密码输入
    mvwgetnstr(loginwin, 3, 11, username, 30);
    mvwgetnstr(loginwin, 5, 11, password, 30);

    // 这里可以添加验证用户名和密码的逻辑
    mvwprintw(loginwin, 7, 1, "Press any key to return to main menu");
    wrefresh(loginwin);  // 刷新窗口以显示所有内容
    getch();  // 等待用户按键

    delwin(loginwin);  // 删除窗口
}

// 显示注册界面的函数
void register_screen() {
    clear();   // 清屏
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);  // 获取终端的最大行列数

    // 创建一个新的窗口用于注册界面
    WINDOW *regwin = newwin(12, 40, (yMax / 2) - 6, (xMax / 2) - 20);
    draw_border(regwin);  // 绘制窗口边框

    // 在窗口中打印提示信息
    mvwprintw(regwin, 1, 1, "Register");
    mvwprintw(regwin, 3, 1, "Username: ");
    mvwprintw(regwin, 5, 1, "Password: ");
    mvwprintw(regwin, 7, 1, "Confirm:  ");

    char username[30], password[30], confirm[30];
    // 获取用户名、密码和确认密码输入
    mvwgetnstr(regwin, 3, 11, username, 30);
    mvwgetnstr(regwin, 5, 11, password, 30);
    mvwgetnstr(regwin, 7, 11, confirm, 30);

    // 这里可以添加注册用户的逻辑
    if (strcmp(password, confirm) == 0) {  // 检查密码和确认密码是否匹配
        mvwprintw(regwin, 9, 1, "Registration successful");
    } else {
        mvwprintw(regwin, 9, 1, "Passwords do not match");
    }

    mvwprintw(regwin, 10, 1, "Press any key to return to main menu");
    wrefresh(regwin);  // 刷新窗口以显示所有内容
    getch();  // 等待用户按键

    delwin(regwin);  // 删除窗口
}

// 主函数
int main() {
    initscr();   // 初始化ncurses
    noecho();    // 禁止在终端上回显用户输入
    cbreak();    // 允许立即读取用户输入，而不需要按Enter键

    while (1) {
        clear();  // 清屏
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);  // 获取终端的最大行列数

        // 打印主菜单
        mvprintw(yMax / 2 - 2, xMax / 2 - 5, "1. Login");
        mvprintw(yMax / 2, xMax / 2 - 5, "2. Register");
        mvprintw(yMax / 2 + 2, xMax / 2 - 5, "3. Exit");

        int choice = getch();  // 获取用户的选择

        // 根据用户的选择调用相应的函数
        switch (choice) {
            case '1':
                login_screen();
                break;
            case '2':
                register_screen();
                break;
            case '3':
                endwin();  // 结束ncurses模式
                return 0;
        }
    }

    endwin();  // 结束ncurses模式
    return 0;
}

