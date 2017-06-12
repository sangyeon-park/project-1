#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

char player[6][30][30], c, input_char;
int s_count = 0, new_count = 0, name_count = 0, name[10] = {0}, mcount = 0, before = 0,line=0 ,row=0, count=0, Ocount = -1, O[20] = {0}, dolcount=-1, maporder=1, end = 0;
// s_count는 e를 눌러 종료시킬때 저장이되어있으면 종료되도록 이용하는 변수이다. new_count는 n을 눌러 맵을처음부터 하도록 이용하는 변수이다.
//name_count는 이름을 입력받기위한 조건문을 위해 사용되는 변수이다. before은 undo를 하기위한 변수이다.
//line과 row는플레이어인 @의 위치를 나타내는 변수이다.
//name[10]은 이름을 받는 배열이고 10자만 받게 크기를 설정 하였다.
//mcount 변수는 map.txt 파일 안의 map 문자 중 m 까지 읽기 위해 지정한 변수
//dolcount는 빈 공간에 들어가 있는 박스 개수를 뜻한다.
//maporder는 맵의 순서를 지정하기 위한 변수 이다.//O[20]은 빈 공간의 위치를 알려주기 위한 배열이고 Ocount는 빈 공간의 개수를 뜻하는 변수이다. 
//Ocount가 0이 되면 스테이지를 클리어 한것이 된다.
//O[20]은 빈 공간의 위치를 알려주기 위한 배열이고 Ocount는 빈 공간의 개수를 뜻하는 변수이다. Ocount가 0이 되면 스테이지를 클리어 한것이 된다.
void name_put(void){//name_count가 0일때는 처음 이름을 입력받게 한다. 그다음 name_count가 1이 되면 입력을 받지 못하게 한다. 
	if(name_count == 0){   
		name_count++;  
		printf("Starting...\n");  
		sleep(1);  
		printf("input name:");  
		for (int i=0; i<=9; i++){     
			name[i] = getchar();// 이름을 입력받아 name배열에 저장      
			if (name[i] == 8){               
				name[i-1] = 0;              
				i-2;}     
			if (name[i] == '\n')              
				break; 
		} 
		printf("Hello ");  
		for(int i = 0; i <= 9; i++){  
			if(name[i] == 0)     
				printf("");   
			else       
				printf("%c",name[i]);
		}  sleep(1);
	}
}

int getch(void){//커맨드가 아래에서 글자가 보이지 않도록 하는 함수이다.
	int ch;
	struct termios buf;
	struct termios save;
	tcgetattr(0 , &save);
	buf = save;
	buf.c_lflag&=~(ICANON|ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void shadow(void){//undo를 위한 함수이며 이동할 때 마다 이전 배열이 저장되어 배정할 수 있게 한다. 
	for(int i = 0; i <= 30 *30 - 1; i++){    
		for(int k = 4; k > -1; k--)      
			player[k + 1][0][i] = player[k][0][i]; 
	}
}

void finderror(){//박스의 개수와 보관장소의 개수가 같은지 확인하고 아니면 종료하는 함수이다.   
	int checkO = 0, checkdol = 0; // O의 개수와 dollar의 개수를 확인하기 위한 변수를 선언했으며, 0으로 초기화했다. 
	for(int i = 0 ; i < 30 ; i++)   //맵을 읽은 후 O의 개수와 $의 개수를 확인한다.    
		for(int j = 0 ; j < 30 ; j++){      
			if(player[0][i][j] == 'O')          
				checkO++;       
			if(player[0][i][j] == '$')          
				checkdol++;  
		}
  if(checkO != checkdol){  // $와 O가 같지 않으면 다음을 출력하고 종료한다.  
	  printf("보관장소와 박스의 개수가 일치하지 않습니다(오류)");  exit(1);
  }
}





void map_memorize(char player[6][box_row][box_line]){
	for (int i=0 ; i<=box_row * box_line - 2 ; i++)
		player[0][0][i] = memorize [0][0][i];
		for (int k=0; k<=4; k++){
		for(int i = 0; i <= box_row * box_line-2; i++)
			player[k+1][0][i] = player[k][0][i];
}
}

void map_load(int a){//map파일을 읽게 하는 함수이며 map_display라는 함수에서 출력하기 위한 배열을 배정한다.
   char letter;
   FILE *fp1;
   int mapcheck = a;
   int x = 0,y =0;
   mcount = 0;
   if(a == 0){//a가 0이라면 저장된파일을 불러옴.
     before = 0, line = 0, row = 0, count = 0, Ocount = -1, dolcount = -1;
     for(int i = 0; i<20; i++)
      O[i] = 0;
     fp1 = fopen("sokoban.txt", "r");//sokoban파일을 읽기 전용으로 열어줌.
     mapcheck = maporder;
   }
   else
      fp1 = fopen("map.txt", "r");//map파일을 읽기 전용으로 열어줌.
      // letter = getc(fp1);
      // player[0][0][0] =letter;
  if (a==0){
    for (int i = 0; i<30*30; i++)
    player[0][0][i] = 0;
    while ((letter = getc(fp1)) != EOF){//fp1으로 읽어들인 문자가 파일의 끝이 아닐때까지 반복함.
      player[0][y][x] = letter;
    if (player[0][y][x] == '@'){
      row=y, line=x;
    }
    if (letter == 'O'){
       Ocount++;
       O[Ocount] = 30*y + x;
       }
       if (player[0][y][x] == '\n'){
         y++;
         x=0;
       }
         else
              x++;
  }
for(int k = 0; k < 5; k++){
 for(int i = 0; i < 30 * 30 - 1; i++)
    player[k + 1][0][i] = player[k][0][i];
  }
}
  else{
     while ((letter = getc(fp1)) != EOF){
       if(letter == 'm'){
             mcount++;
             if(mcount == a+1)
                break;
             continue;
          }
          if(letter == 'a')
             continue;
          if(letter == 'e'){
             end++;
             break;
          }

       player[0][y][x] = letter;

       if(player [0][y][x] == 'p'){
             x = 0, y = 0, Ocount = -1;
             for(int i = 0; i < 20; i++)
                O[i] = 0;
             for(int i = 0; i < 30 * 30; i++)
                player[0][0][i] = 0;
             continue;
          }

     if (player[0][y][x] == '@'){
       row=y, line=x;
     }

        if (letter == 'O'){
           Ocount++;
           O[Ocount] = 30*y + x;
           }
           if (player[0][y][x] == '\n'){
             y++;
             x=0;
           }
             else
                          x++;
     }
     for(int k = 0; k < 5; k++){
      for(int i = 0; i < 30 * 30 - 1; i++)
         player[k + 1][0][i] = player[k][0][i];
       }
     fclose(fp1);
   }
 }

void map_display(){//게임의 진행 현황을 표시하는 함수이며 기본적인 배열은 player[0]을 사용한다.
   system("clear");
   printf("hello  ");
   for(int i = 0; i <= 9; i++){
     if(name[i] == 0)
       printf(" ");
     else
         printf("%c",name[i]);//입력받은 이름을 출력시키기 위해서 반복문을 사용함
   }
   printf("\n\n");
   for (int y = 0; y< 30; y++ ){
     for (int x=0; x< 30; x++){
       if (player[0][y][x] != 0)
       printf ("%c", player[0][y][x]);
     }
   }
   printf("\n");
}

void shadow(void){//undo를 위한 함수이며 이동할 때 마다 이전 배열이 저장되어 배정할 수 있게 한다.
  for(int i = 0; i <= 30 *30 - 1; i++){
     for(int k = 4; k > -1; k--)
        player[k + 1][0][i] = player[k][0][i];
  }
}




int move(void) {//@의 행동을 총괄하는 함수이며 커맨드를 입력받은 후 switch를 통해 원하는 기능으로 제어가 넘어가도록 한다.
   input_char = getch();// getch를 사용하여 키보드로 input_char에 문자를 입력받음.
   switch (input_char) {
      case 'u' : {//undo의 역할을 하는 case이며 shadow를 거꾸로 실행한다고 생각하면 쉽다, 배열을 거꾸로 배정하여 이전의 위치로 돌아갈 수 있도록 만든다.
               if(before < 5){
                  for(int i = 0; i <= 30*30-1; i++){
                     for(int k = 0; k <= 4; k++)
                        player[k][0][i] = player[k + 1][0][i];
                  }
                  for(int i = 0; i <=29; i++){
                     for(int j = 0; j <=29; j++){
                        if(player[0][i][j] == '@'){
                           row = i;
                           line = j;
                           break;
                        }
                     }
                  }
                  before++;
               }
            }
            break;
            case 'n': {// 새로히 게임을 실행할 수 있게하는 case이며 undo의 가능횟수를 초기화 하고 main함수에서 새로히 재귀함수의 개념으로 게임이 실행되도록 하기위해 간접적으로
              //변수(new_count)를 조절하는 방법을 취했다
               printf("n\n");
               sleep(1);
               printf("1단계 맵부터 다시 시작합니다\n");
               sleep(1);
               new_count++;
               maporder = 1, before = 0, line = 0, row = 0, count = 0, Ocount = -1;
               for (int i = 0; i <= 19; i++)
                  O[i] = 0;
               break;
            }
            case 'e': {// 게임을 종료하는 기능으로 저장이 안되어 있다면 종료를 할 수 없게 s_count라는 변수를 두어 제어하였다.
               printf("e\n");
               sleep(1);
               if (s_count >= 1) {
                  printf("SEE YOU...  ");
                  for(int i = 0; i <= 9; i++){
                    if(name[i] == 0)
                      printf("");
                    else
                        printf("%c",name[i]);
                  }
                  sleep(1);
                  exit(1);
               }
               if (s_count == 0)
                  printf("저장하고 종료해주세요\n");
               sleep(1);
               break;
            }
      case 'd':{//도움말을 보여주는 case이며 printf함수를 이용하여 커맨드의 역할을 제시하였다, getch()를 통해 도움말에서 벗어날 수
        //있도록 만들었다.
             printf("k \n");
             printf("도움말을 보여줍니다 \n");
             sleep(1);
             system("clear");
             printf("\n\n\n\nh(왼쪽), j(아래), k(위), l(오른쪽)\n");
             printf("u(undo)\n");
             printf("r(replay)\n");
             printf("n(new)\n");
             printf("e(exit)\n");
             printf("s(save)\n");
             printf("f(file load)\n");
             printf("d(display help)\n");
             printf("t(top)\n\n\n\n\n\n");
             printf("맵으로 돌아가실거면 아무키나 눌러주세요\n");
             getch();
             system("clear") ;
             map_display(player);
          }
          break;
      case 'l' : {//case 'l', case'k', case'j', case'h'는 상하좌우로 이동하게하는 기능이며 제어구문을 통해 장애물에 맞닥뜨릴때의 경우에 따라 움직일 수 없게 break를 두었다.
        //그리고 움직임으로 인해 배열에 변화를 처리 하기 위해 움직임이 반영될 수 있는 경우에 shadow함수를 먼저 두어 undo를 위해
        //현재의 상태를 다른 배열에 저장해 두고 배열을 바꾸었다.
               if(player[0][row][line+1] == '#') //플레이어가 이동해야할 좌표가 벽이라면 움직이지 않음.
                  break;
               if(player[0][row][line+1] == '$')//플레이어가 이동해야할 좌표에 박스있는경우
               {
                  if(player[0][row][line+2] == '#' || player[0][row][line+2] == '$')//플레이어가 상자를 밀었을때 상자의 위치에 벽이나 상자가 있으면 플레이어와 상자 모두 움직이지 않음.
                     break;
                  shadow();
                  line += 1;
                  player[0][row][line-1] = ' ';
                  player[0][row][line] = '@';
                  player[0][row][line+1] = '$';
                  break;
               }
               shadow();
               player[0][row][line] =' ';
               line+=1;
               player[0][row][line] ='@';

            }
            break;

      case 'h' : {
               if(player[0][row][line-1] == '#')//플레이어가 이동해야할 좌표가 벽이라면 움직이지 않음.
                  break;
               if(player[0][row][line-1] == '$')//플레이어가 이동해야할 좌표에 박스있는경우
               {
                  if(player[0][row][line-2] == '#' || player[0][row][line-2] == '$')//플레이어가 상자를 밀었을때 상자의 위치에 벽이나 상자가 있으면 플레이어와 상자 모두 움직이지 않음.
                     break;
                   shadow();
                  line -= 1;
                  player[0][row][line+1] = ' ';
                  player[0][row][line] = '@';
                  player[0][row][line-1] = '$';
                  break;
               }
               shadow();
               player[0][row][line] =' ';
               line-=1;
               player[0][row][line] ='@';
            }
            break;
      case 'k' : {
               if(player[0][row-1][line] == '#')//플레이어가 이동해야할 좌표가 벽이라면 움직이지 않음.
                  break;
               if(player[0][row-1][line] == '$')//플레이어가 이동해야할 좌표에 박스있는경우
               {
                  if(player[0][row-2][line] == '#' || player[0][row-2][line] == '$')//플레이어가 상자를 밀었을때 상자의 위치에 벽이나 상자가 있으면 플레이어와 상자 모두 움직이지 않음.
                     break;
                   shadow();
                  row -= 1;
                  player[0][row+1][line] = ' ';
                  player[0][row][line] = '@';
                  player[0][row-1][line] = '$';
                  break;
               }
               shadow();
               player[0][row][line] =' ';
               row-=1;
               player[0][row][line] ='@';
            }
            break;

      case 'j' : {
               if(player[0][row+1][line] == '#')//플레이어가 이동해야할 좌표가 벽이라면 움직이지 않음.
                  break;
               if(player[0][row+1][line] == '$')//플레이어가 이동해야할 좌표에 박스있는경우
               {
                  if(player[0][row+2][line] == '#' || player[0][row+2][line] == '$')//플레이어가 상자를 밀었을때 상자의 위치에 벽이나 상자가 있으면 플레이어와 상자 모두 움직이지 않음.
                     break;
                  shadow();
                  row += 1;
                  player[0][row-1][line] = ' ';
                  player[0][row][line] = '@';
                  player[0][row+1][line] = '$';
                  break;
               }
               shadow();
               player[0][row][line] =' ';
               row+=1;
               player[0][row][line] ='@';
            }
            break;
             case 's' :{
               FILE *sfp;
               sfp = fopen("sokoban.txt", "w");//sokoban 파일을 쓰기전용으로 열어줌.
               printf("s \n");
               sleep(1);
               printf("파일을 저장했습니다\n");
               sleep(1);
               fprintf(sfp, "%d\n", maporder);
               for (int ho=0; ho<=30*30; ho++){
                 if (player[0][0][ho] != 0){
                  fprintf(sfp,"%c", player[0][0][ho]);
               }
             }
               fclose(sfp);
               s_count++;
               break;
             }
      case 'f' : {//파일을 불러오는 기능이며 map_load라는 함수에서 다른 값을 집어넣어 저장된 배열을 불러올 수 있게 만들었다.
               printf("f \n");
               sleep(1);
               printf("저장된 파일을 불러오겠습니다\n");
               sleep(1);
                system("clear");
               map_load(0);
               map_display();
               break;
            }

      case 'r' : {//현재의 스테이지에서 처음부터 새로 하기위해 위치를 초기화 하였다.
        //case'n'과 다른 점은 진행중인 스테이지만 초기화 하면 되기 때문에 현재 불러온 배열만 초기화 한 후 에 map_display를 사용하면 되었다.
               printf("r\n");
               sleep(1);
               printf("처음부터 다시 시작합니다\n");
               sleep(1);
               line=0 ,row=0, Ocount = -1;
               for(int i=0; i<=19; i++)
                  O[i] = 0;
               map_load(maporder);
               system("clear");
               map_display();
               break;
            }
            case 'v' : {//다음 스테이지로 넘어가게하는 case이며 가장 아래에 위치해 있기 때문에 break를 따로 두지 않았다.
              //다음 맵으로 이동하게 Ocount를 -1 로초기화 하였다.
              Ocount = -1;
            }
      default : break;
   }
   return 0;
}




int main(void)
{
  system("clear");
   name_put();
   map_load(maporder);
   map_display();
   while (1){
      move ();
      if (new_count == 1) {//move함수에서 case'n'을 위해서
         new_count--;
         main();
      }
      dolcount = -1;      //O도 0부터 셌기 때문에 $ 도 0부터 세기 위해
      for(int n=0;n<=Ocount;n++){
         if(player[0][0][O[n]] == ' ' || player[0][0][O[n]] == 0)
            player[0][0][O[n]] = 'O';
         if(player[0][0][O[n]] == '$')
            dolcount++;
      }
      map_display();
      if(dolcount == Ocount){
         printf("\n다음 스테이지로 넘어갑니다.\n"); // 이 부분에서 맵이 넘어감. 추가할 부분
         sleep(1);
         maporder++;
         main();
       }
   }
   return 0;
}
