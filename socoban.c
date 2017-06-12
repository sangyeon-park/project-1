#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



char memorize[6][30][30], c, input_char;
int before = 0,line=0 ,row=0, box_line=1 ,box_row=1, count=0, Ocount = -1, O[20] = {0}, dolcount=-1;
time_t start, end;



int getch(void){
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



void map_load(int a){
	char letter;
	FILE *fp1;
	if (a == 0)
		fp1 = fopen("jong.txt", "r");
		if (a==1){
			before = 0,line=0 ,row=0, box_line=1 ,box_row=1, count=0, Ocount = -1, dolcount=-1;
			for (int i =0; i<=19; i++)
			O[i] = 0;
			fp1 = fopen("sokoban.txt", "r");}

	while ((letter = getc(fp1)) != EOF){
		memorize[0][0][count]=letter;
		count++;
		if (letter == '\n')
			++box_row;
		if (box_row==1 )
			++box_line;
		if (letter=='@'){
			row = box_row-1;
			line = (count+1) % box_line-2;
		}
		if (letter == 'O'){
			Ocount++;
			O[Ocount] = count-1;
		}
	}
	fclose(fp1);
}


void map_memorize(char player[6][box_row][box_line]){
	for (int i=0 ; i<=box_row * box_line - 2 ; i++)
		player[0][0][i] = memorize [0][0][i];
		for (int k=0; k<=4; k++){
		for(int i = 0; i <= box_row * box_line-2; i++)
			player[k+1][0][i] = player[k][0][i];
}
}



void map_display(char player[6][box_row][box_line]){
	system("clear");
	for(int i=0;i <= box_row * box_line - 2;i++){
		if (player[0][0][i] == 0)
			printf("");
		else
			printf("%c", player[0][0][i]);
	}
	printf("%d %d %d %d", row, line, box_row, box_line);
	printf("\n");
	return;
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
