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
//mcount 변수는 map.txt 파일 안의 map 문자 중 m 까지 읽기 위해 지정한 변수//dolcount는 빈 공간에 들어가 있는 박스 개수를 뜻한다.
//maporder는 맵의 순서를 지정하기 위한 변수 이다.//O[20]은 빈 공간의 위치를 알려주기 위한 배열이고 Ocount는 빈 공간의 개수를 뜻하는 변수이다. Ocount가 0이 되면 스테이지를 클리어 한것이 된다.

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


int move(char player[6][box_row][box_line]) {
	input_char = getch();
	switch (input_char) {
		case 'u' : {
				   if(before < 5){
					   for(int i = 0; i <= box_row * box_line - 2; i++){
						   for(int k = 0; k <= 4; k++)
							   player[k][0][i] = player[k + 1][0][i];
					   }
					   for(int i = 0; i <= box_row; i++){
						   for(int j = 0; j <= box_line; j++){
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

		case 'k':{
				 printf("k \n");
				 printf("도움말을 보여줍니다 \n");
				 sleep(1);
				 system("clear");
				 printf("\n\n\n\na(왼쪽), s(아래), w(위), d(오른쪽)\n");
				 printf("u(undo)\n");
				 printf("r(replay)\n");
				 printf("n(new)\n");
				 printf("e(exit)\n");
				 printf("l(save)\n");
				 printf("f(file load)\n");
				 printf("k(display help)\n");
				 printf("t(top)\n\n\n\n\n\n");
				 printf("맵으로 돌아가실거면 아무키나 눌러주세요\n");

				 getch();
				 system("clear") ;

				 map_display(player);

			 }
			 break;

		case 'd' : {
				   if(player[0][row][line+1] == '#')
					   break;
				   if(player[0][row][line+1] == '$')
				   {
					   if(player[0][row][line+2] == '#' || player[0][row][line+2] == '$')
						   break;
					   for(int i = 0; i <= box_row * box_line - 2; i++){
						   for(int k = 4; k > -1; k--)
							   player[k + 1][0][i] = player[k][0][i];
					   }
					   line += 1;
					   player[0][row][line-1] = ' ';
					   player[0][row][line] = '@';
					   player[0][row][line+1] = '$';
					   break;
				   }
				   for(int i = 0; i <= box_row * box_line - 2; i++){
					   for(int k = 4; k > -1; k--)
						   player[k + 1][0][i] = player[k][0][i];
				   }
				   player[0][row][line] =' ';
				   line+=1;
				   player[0][row][line] ='@';

			   }
			   break;

		case 'a' : {
				   if(player[0][row][line-1] == '#')
					   break;
				   if(player[0][row][line-1] == '$')
				   {
					   if(player[0][row][line-2] == '#' || player[0][row][line-2] == '$')
						   break;
					   for(int i = 0; i <= box_row * box_line - 2; i++){
						   for(int k = 4; k > -1; k--)
							   player[k + 1][0][i] = player[k][0][i];
					   }
					   line -= 1;
					   player[0][row][line+1] = ' ';
					   player[0][row][line] = '@';
					   player[0][row][line-1] = '$';
					   break;
				   }
				   for(int i = 0; i <= box_row * box_line - 2; i++){
					   for(int k = 4; k > -1; k--)
						   player[k + 1][0][i] = player[k][0][i];
				   }
				   player[0][row][line] =' ';
				   line-=1;
				   player[0][row][line] ='@';
			   }
			   break;


		case 'w' : {
				   if(player[0][row-1][line] == '#')
					   break;
				   if(player[0][row-1][line] == '$')
				   {
					   if(player[0][row-2][line] == '#' || player[0][row-2][line] == '$')
						   break;
					   for(int i = 0; i <= box_row * box_line - 2; i++){
						   for(int k = 4; k > -1; k--)
							   player[k + 1][0][i] = player[k][0][i];
					   }
					   row -= 1;
					   player[0][row+1][line] = ' ';
					   player[0][row][line] = '@';
					   player[0][row-1][line] = '$';
					   break;
				   }
				   for(int i = 0; i <= box_row * box_line - 2; i++){
					   for(int k = 4; k > -1; k--)
						   player[k + 1][0][i] = player[k][0][i];
				   }
				   player[0][row][line] =' ';
				   row-=1;
				   player[0][row][line] ='@';
			   }
			   break;

		case 's' : {
				   if(player[0][row+1][line] == '#')
					   break;
				   if(player[0][row+1][line] == '$')
				   {
					   if(player[0][row+2][line] == '#' || player[0][row+2][line] == '$')
						   break;
					   for(int i = 0; i <= box_row * box_line - 2; i++){
						   for(int k = 4; k > -1; k--)
							   player[k + 1][0][i] = player[k][0][i];
					   }
					   row += 1;
					   player[0][row-1][line] = ' ';
					   player[0][row][line] = '@';
					   player[0][row+1][line] = '$';
					   break;
				   }
				   for(int i = 0; i <= box_row * box_line - 2; i++){
					   for(int k = 4; k > -1; k--)
						   player[k + 1][0][i] = player[k][0][i];
				   }
				   player[0][row][line] =' ';
				   row+=1;
				   player[0][row][line] ='@';
			   }
			   break;

			    case 'l' :{
			      FILE *sfp;
			      sfp = fopen("sokoban.txt", "w");
			      printf("l \n");
			      sleep(1);
			      printf("파일을 저장했습니다\n");
			      sleep(1);
			      for (int ho=0; ho<=box_line*box_row; ho++){
			         fprintf(sfp,"%c", player[0][0][ho]);
			      }
			      fclose(sfp);
			      break;
			    }


		case 'f' : {
				   printf("f \n");
				   sleep(1);
				   printf("저장된 파일을 불러오겠습니다\n");
				   sleep(1);
					 system("clear");
				   map_load(1);
				   map_memorize(player);
				   map_display(player);
				   break;
			   }

		case 'r' : {
				   printf("r\n");
				   sleep(1);
				   printf("처음부터 다시 시작합니다\n");
				   sleep(1);
				   line=0 ,row=0, box_line=1 ,box_row=1, count=0, Ocount = -1;
				   for(int i=0; i<=19; i++)
					   O[i] = 0;
				   map_load(0);
				   map_memorize(player);
				   system("clear");
				   map_display(player);
				   break;
			   }
		default : break;
	}
	return 0;
}




int main(void)
{
	system("clear");
	map_load(0);
	char player[6][box_row][box_line];
	map_memorize(player);
	map_display(player);
	while (1){
		move (player);
		system("clear");
		dolcount = -1;      //O도 0부터 셌기 때문에 $ 도 0부터 세기 위해
		for(int n=0;n<=Ocount;n++){
			if(player[0][0][O[n]] == ' ' || player[0][0][O[n]] == 0)
				player[0][0][O[n]] = 'O';
			if(player[0][0][O[n]] == '$')
				dolcount++;
		}
		system("clear");
		map_display(player);
		if(dolcount == Ocount)
			printf("다음 스테이지로 넘어갑니다."); // 이 부분에서 맵이 넘어감. 추가할 부분
	}
	return 0;
}
