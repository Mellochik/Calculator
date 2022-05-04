#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"

//структура кнопок
typedef struct button {
   int x, y; //координаты кнопки
   int width, height; //ширина и длина кнопки
   char name[10]; //имя кнопки
   int type; //тип кнопки
   int number; //числое значение кнопки
   int textxy[2]; //коориднаты надписи
} button;

//стркутура переключателей сс
typedef struct toggle {
   int x, y; //координаты переключателя
   char name[4]; //имя переключателя
   int ss; //система счисления переключателя
} toggle;

//структура счётчика
typedef struct tablet {
   char text1[17]; //первая строка вывода
   char text2[17]; //вторая строка вывода
} tablet;

void render_button(); //отрисовка кнопок
void render_toggle(); //отрисовка переключателся системы счисления
void render_button_BIN(int i); //отрисовка кнопок для двоичной системы счисления
void render_button_OCT(int i); //отрисовка кнопок для восьмиричной системы счисления
void render_button_DEC(int i); //отрисовка кнопок для десятеричной системы счисления
void render_button_HEX(int i); //отрисовка кнопок для шестнадцатиричной системы счисления
void click_button(int x, int y); //отслеживание нажатой кнопки
void click_number_button(int i); //отслеживание кнопки цифры
void click_operation_button(int i); //нажатие кнопки операции
void add_temp(int i); //добавление в числовую переменную цифры
void presure_button(int i); //отрисовка нажатия клавиши
void update_scoreboard(); //обновление счетчика
void print_in_ss(int n); //вывод в данной системы счисления
void init_scoreboard(); //инициализация счетчика
void clear_scoreboard(); //очитска экрана
void operation(int oper); //выполнение операции
void render_button_up(int i); //отрисовка не нажатой кнопки
void render_button_down(int i); //отрисовка нажатой кнопки
void render_button_disabled(int i); //отрисовка неактивной кнопки
int current_ss(int i); //определяет какая сейчас системы счисления
void init_operation(int i); //определяет была ли уже нажата кнопка операции
void plus_minus_scoreboard(); //изменение знака числа на экране
void second_ss(int n); //перевод в двоичную системы счисления
void reverse(char s[]);//переворот числа

int answer = 0; //переменная ответа
int temp = 0; //переманная ввода
int toggle_ss = 10; //ныняшняя системы счисления
int last_operation = -1; //последняя операция
int x, y; //координаты мышки на экране
tablet table; //счетчик
int answer_push = 0; //нажата ли была клавиша "="
//кнопки
button b[28] = {{4, 140, 116, 106,"D", 1, 13, {50, 172}},
   {124, 140, 116, 106,"E", 1, 14, {170, 172}},
   {244, 140, 116, 106,"F", 1, 15, {290, 172}},
   {364, 140, 116, 106,"AC", 2, 0, {400, 172}},
   {484, 140, 116, 106,"C", 2, 0, {530, 172}},
   {4, 250, 116, 106,"A", 1, 10, {50, 282}},
   {124, 250, 116, 106,"B", 1, 11, {170, 282}},
   {244, 250, 116, 106,"C", 1, 12, {290, 282}},
   {364, 250, 116, 106,"AND", 2, 0, {384, 282}},
   {484, 250, 116, 106,"OR", 2, 0, {520, 282}},
   {4, 360, 116, 106,"7", 1, 7, {50, 392}},
   {124, 360, 116, 106,"8", 1, 8, {170, 392}},
   {244, 360, 116, 106,"9", 1, 9, {290, 392}},
   {364, 360, 116, 106,"NOT", 2, 0, {384, 392}},
   {484, 360, 116, 106,"XOR", 2, 0, {504, 392}},
   {4, 470, 116, 106,"4", 1, 4, {50, 502}},
   {124, 470, 116, 106,"5", 1, 5, {170, 502}},
   {244, 470, 116, 106,"6", 1, 6, {290, 502}},
   {364, 470, 116, 106,"/", 3, 0, {410, 502}},
   {484, 470, 116, 106,"-", 3, 0, {530, 502}},
   {4, 580, 116, 106,"1", 1, 1, {50, 612}},
   {124, 580, 116, 106,"2", 1, 2, {170, 612}},
   {244, 580, 116, 106,"3", 1, 3, {290, 612}},
   {364, 580, 116, 106,"X", 3, 0, {410, 612}},
   {484, 580, 116, 106,"+", 3, 0, {530, 612}},
   {4, 690, 236, 106,"0", 1, 0, {110, 722}},
   {244, 690, 116, 106,"+/-", 1, 0, {264, 722}},
   {364, 690, 236, 106,"=", 3, 0, {470, 722}},
};
//переключатели системы счисления
toggle t[4] = {{555, 20, "BIN", 2},
   {555, 45, "OCT", 8},
   {555, 70, "DEC", 10},
   {555, 95, "HEX", 16},
};

int main() {
   initwindow(606, 800, "Калькулятор для программиста", 100, 100);
   setbkcolor(COLOR(110, 110, 110));
   clearviewport();
   render_button();
   render_toggle();
   init_scoreboard();
   update_scoreboard();

   while (1) {
      x = mousex();
      y = mousey();
      if (mousebuttons() == 1)
         click_button(x, y);
   }

   getch();
   closegraph();
}

//отрисовка кнопок
void render_button() {
   for (int i = 0; i < 28; i++) {
      if (toggle_ss == 2)
         render_button_BIN(i);
      else if (toggle_ss == 8)
         render_button_OCT(i);
      else if (toggle_ss == 10)
         render_button_DEC(i);
      else render_button_HEX(i);
   }
}

//отрисовка кнопок для двоичной системы счисления
void render_button_BIN(int i) {
   if (i >= 0 && i <= 2 || i >= 5 && i <= 7 || i >= 10 && i <= 12 || i >= 15 && i <= 17 || i >= 21 && i <= 22) {
      render_button_disabled(i);
   }
   else render_button_up(i);
}

//отрисовка кнопок для восьмиричной системы счисления
void render_button_OCT(int i) {
   if (i >= 0 && i <= 2 || i >= 5 && i <= 7 || i == 11 || i == 12) {
      render_button_disabled(i);
   }
   else render_button_up(i);
}

//отрисовка кнопок для десятеричной системы счисления
void render_button_DEC(int i) {
   if (i >= 0 && i <= 2 || i >= 5 && i <= 7) {
      render_button_disabled(i);
   }
   else render_button_up(i);
}

//отрисовка кнопок для шестнадцатиричной системы счисления
void render_button_HEX(int i) {
   render_button_up(i);
}

//отрисовка переключателся системы счисления
void render_toggle() {
   for (int i = 0; i < 4; i++) {
      if (t[i].ss != toggle_ss) {
         setcolor(COLOR(255, 255, 255));
         setbkcolor(COLOR(110, 110, 110));
         settextstyle(8, 0, 3);
         outtextxy(t[i].x, t[i].y, t[i].name);
      }
      else { //если выбрана системы счисления, то ее кнопку отрисовывает темным цветом
         setcolor(COLOR(150, 150, 150));
         setbkcolor(COLOR(110, 110, 110));
         settextstyle(8, 0, 3);
         outtextxy(t[i].x, t[i].y, t[i].name);
      }
   }
}

//отслеживание нажатой кнопки
void click_button(int x, int y) {
   for (int i = 0; i < 28; i++)
      if (b[i].x <= x && b[i].x + b[i].width >= x && b[i].y <= y && b[i].y + b[i].height >= y) { //кнопки цифр
         answer_push = 0;
         click_number_button(i);
         clearviewport();
         render_toggle();
         render_button();
         clear_scoreboard();
         print_in_ss(temp);
         if (i == 3) { //кнопка "АС"
            presure_button(i);
            answer = 0;
            temp = 0;
            last_operation = -1;
            clear_scoreboard();
         }
         else if (i == 4) { //кнопка "С"
            presure_button(i);
            temp = 0;
            clear_scoreboard();
         }
         else if (i >= 8 && i <= 9 || i == 14 || i >= 18 && i <= 19 || i >= 23 && i <= 24) { //кнпки операций
            presure_button(i);
            click_operation_button(i);
         }
         else if (i == 13) {
            presure_button(i);
            temp = ~temp;
            clear_scoreboard();
            update_scoreboard();
            print_in_ss(temp);
         }
         else if (i == 26) { //кнопка изменения знака
            presure_button(i);
            plus_minus_scoreboard();
            clear_scoreboard();
            update_scoreboard();
            print_in_ss(temp);
         }
         else if (i == 27) { //кнопка "="
            answer_push = 1;
            operation(last_operation);
            clear_scoreboard();
            presure_button(i);
            clear_scoreboard();
            update_scoreboard();
            print_in_ss(answer);
         }
         update_scoreboard();
      }
   for (int i = 0; i < 4; i++) {
      if (t[i].x <= x && t[i].x + 40 >= x && t[i].y <= y && t[i].y + 20 >= y) {
         if (toggle_ss != current_ss(i)) { //чтобы нельзя было нажать еще раз на эту кнопку
            toggle_ss = current_ss(i);
            clearviewport();
            render_toggle();
            render_button();
            clear_scoreboard();
            update_scoreboard();
            if (answer_push != 0)
               print_in_ss(answer);
            else
               print_in_ss(temp);
            update_scoreboard();
         }
      }
   }
}

//определяет какая сейчас системы счисления
int current_ss(int i) {
   if (i == 0) return 2;
   else if (i == 1) return 8;
   else if (i == 2) return 10;
   else if (i == 3) return 16;
   return 0;
}

//нажатие кнопки операции
void click_operation_button(int i) {
   if (toggle_ss != 10) {
      init_operation(i);
      clear_scoreboard();
   }
   else init_operation(i);
   clear_scoreboard();
   temp = 0;
}

//определяет была ли уже нажата кнопка операции
void init_operation(int i) {
   if (answer == 0) {
      answer = temp;
      last_operation = i;
   }
   else {
      operation(last_operation);
      last_operation = i;
   }
}

//отслеживание кнопки цифры
void click_number_button(int i) {
   if (toggle_ss == 2) {
      if (i == 20 || i == 25) {
         presure_button(i);
         add_temp(i);
         update_scoreboard();
      }
   }
   else if (toggle_ss == 8) {
      if (i == 10 || i >= 15 && i <= 17 || i >= 20 && i <= 22 || i == 25) {
         presure_button(i);
         add_temp(i);
         update_scoreboard();
      }
   }
   else if (toggle_ss == 10) {
      if (i >= 10 && i <= 12 || i >= 15 && i <= 17 || i >= 20 && i <= 22 || i == 25) {
         presure_button(i);
         add_temp(i);
         update_scoreboard();
      }
   }
   else if (toggle_ss == 16) {
      if (i >= 0 && i <= 2 || i >= 5 && i <= 7 || i >= 10 && i <= 12 || i >= 15 && i <= 17 || i >= 20 && i <= 22 || i == 25) {
         presure_button(i);
         add_temp(i);
         update_scoreboard();
      }
   }
}

//отрисовка нажатия клавиши
void presure_button(int i) {
   render_button_down(i);
   delay(100);
   render_button_up(i);
   delay(100);
}

//добавление в числовую переменную цифры
void add_temp(int i) {
   temp *= toggle_ss;
   temp += b[i].number;
}

//выполнение операции
void operation(int oper) {
   if (oper == 8) {
      answer &= temp;
   }
   else if (oper == 9) {
      answer |= temp;
   }
   else if (oper == 14) {
      answer ^= temp;
   }
   else if (oper == 18) {
      if (temp != 0)
         answer /= temp;
   }
   else if (oper == 19) {
      answer -= temp;
   }
   else if (oper == 23) {
      answer *= temp;
   }
   else if (oper == 24) {
      answer += temp;
   }
}

//инициализация счетчика
void init_scoreboard() {
   table.text1[0] = '0';
}

//очитска экрана
void clear_scoreboard() {
   table.text1[0] = '0';
   for (int k = 1; k < 16; k++)
      table.text1[k] = ' ';
   table.text1[16] = '\0';
   for (int k = 0; k < 16; k++)
      table.text2[k] = ' ';
   table.text2[16] = '\0';
}

//обновление счетчика
void update_scoreboard() {
   setcolor(COLOR(255, 255, 255));
   setbkcolor(COLOR(110, 110, 110));
   settextstyle(8, 0, 6);
   if (toggle_ss != 2)
      outtextxy(10, 40, table.text1);
   else { //сли двоичная то выводится в две строки
      settextstyle(8, 0, 5);
      outtextxy(10, 25, table.text1);
      outtextxy(10, 65, table.text2);
   }
}

//вывод в данной системы счисления
void print_in_ss(int n) {
   if (toggle_ss == 2)
      second_ss(n);
   else if (toggle_ss == 8)
      sprintf(table.text1, "%o", n);
   else if (toggle_ss == 10)
      sprintf(table.text1, "%d", n);
   else if (toggle_ss == 16)
      sprintf(table.text1, "%X", n);
}

//отрисовка не нажатой кнопки
void render_button_up(int i) {
   if (b[i].type == 1) {
      setfillstyle(SOLID_FILL, COLOR(180, 180, 180));
      setcolor(COLOR(0, 0, 0));
      setbkcolor(COLOR(180, 180, 180));
   }
   else if (b[i].type == 2) {
      setfillstyle(SOLID_FILL, COLOR(160, 160, 160));
      setcolor(COLOR(0, 0, 0));
      setbkcolor(COLOR(160, 160, 160));
   }
   else if (b[i].type == 3) {
      setfillstyle(SOLID_FILL, COLOR(255, 115, 20));
      setcolor(COLOR(255, 255, 255));
      setbkcolor(COLOR(255, 115, 20));
   }
   bar(b[i].x, b[i].y, b[i].x + b[i].width, b[i].y + b[i].height);
   settextstyle(8, 0, 5);
   outtextxy(b[i].textxy[0], b[i].textxy[1], b[i].name);
}

//отрисовка нажатой нкопки
void render_button_down(int i) {
   if (b[i].type == 1) {
      setfillstyle(SOLID_FILL, COLOR(220, 220, 220));
      setcolor(COLOR(0, 0, 0));
      setbkcolor(COLOR(220, 220, 220));
   }
   else if (b[i].type == 2) {
      setfillstyle(SOLID_FILL, COLOR(200, 200, 200));
      setcolor(COLOR(0, 0, 0));
      setbkcolor(COLOR(200, 200, 200));
   }
   else if (b[i].type == 3) {
      setfillstyle(SOLID_FILL, COLOR(255, 150, 80));
      setcolor(COLOR(255, 255, 255));
      setbkcolor(COLOR(255, 150, 80));
   }
   bar(b[i].x, b[i].y, b[i].x + b[i].width, b[i].y + b[i].height);
   settextstyle(8, 0, 5);
   outtextxy(b[i].textxy[0], b[i].textxy[1], b[i].name);
}

//отрисовка неактивной кнопки
void render_button_disabled(int i) {
   setfillstyle(SOLID_FILL, COLOR(140, 140, 140));
   setcolor(COLOR(0, 0, 0));
   setbkcolor(COLOR(140, 140, 140));
   bar(b[i].x, b[i].y, b[i].x + b[i].width, b[i].y + b[i].height);
   settextstyle(8, 0, 5);
   outtextxy(b[i].textxy[0], b[i].textxy[1], b[i].name);
}

//изменение знака числа на экране
void plus_minus_scoreboard() {
   temp *= -1;
}

//перевод в двоичную сс
void second_ss(int n) {
   int z = 0;
   if (n < 0) { //проверка на отрицательное число
      z = 1;
      n *= -1;
      n -= 1;
   }
   int k = 0;
   char s[32];
   while (n > 0) {
      s[k] = (n % 2) + '0';
      k++;
      n /= 2;
   }
   if (z == 1) {
      for (int i = 0; i < k; i++)
         s[i] = '1' - s[i] + '0';
      for (int i = k; i < 32; i++)
         s[i] = '1';
      k = 32;
   }
   char t; //переворот массива
   for (int i = 0; i < k / 2; i++) {
      t = s[i];
      s[i] = s[k - 1 - i];
      s[k - 1 - i] = t;
   }
   for (int i = 0; i < k; i++) { //разделение на две строки
      if (i <= 15) {
         table.text1[i] = s[i];
         table.text1[i + 1] = '\0';
      }
      else {
         table.text2[i - 16] = s[i];
         table.text2[i - 16 + 1] = '\0';
      }
   }
}
