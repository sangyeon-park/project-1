#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
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
        return ch; }



        int main(void){

                char c,letter, input_char;
                FILE *go;
                int line=0 ,row=0, box_line=1 ,box_row=1, count=0;

                go = fopen("jong.txt", "r");  //맵 열기
                char memorize[10][10][10]; // 임시 저장공간
                int Ocount = -1;
                int O[20] = {0}; 


                while ((letter = getc(go)) != EOF){

                        memorize[0][0][count]=letter;
                        count++;



                        if (letter == '\n')
                                ++box_row;
                        if (box_row==1)
                                ++box_line;
                        if (letter=='@'){
                                row = box_row-1;
                                line = (count+1) % box_line-2;
                        }
                         if (letter == 'O')
                        {
                            Ocount += 1;
                            O[Ocount] = count-1;
                        }

                }

                fclose(go);



                char player[box_row][box_line];

                for(int i=0 ; i<=box_row*box_line-2 ;i++ )
                        player[0][i] = memorize[0][0][i];


                system("clear");


                for (int i=0; i<=box_line*box_row; i++)
                        printf("%c", player[0][i]);

                fclose(go);



                while (1){
                        input_char = getch();
                        if (input_char == 'q'){
                                printf("Bye\n");
                                break;      }
                        else{
                                switch (input_char) {
                                        case 'd' : {
                                                           if(player[row][line+1] == '#')
                                                                   break;
                                                           if(player[row][line+1] == '$')
                                                           {
                                                                   if(player[row][line+2] == '#' || player[row][line+2] == '$')
                                                                           break;
                                                                   line += 1;
                                                                   player[row][line-1] = ' ';
                                                                   player[row][line] = '@';
                                                                   player[row][line+1] = '$';
                                                                   break;
                                                           }

                                                           player[row][line] =' ';
                                                           line+=1;
                                                           player[row][line] ='@';

                                                   }

                                                   break;

                                        case 'a' : {
                                                           if(player[row][line-1] == '#')
                                                                   break;
                                                           if(player[row][line-1] == '$')
                                                           {
                                                                   if(player[row][line-2] == '#' || player[row][line-2] == '$')
                                                                           break;
                                                                   line -= 1;
                                                                   player[row][line+1] = ' ';
                                                                   player[row][line] = '@';
                                                                   player[row][line-1] = '$';
                                                                   break;
                                                           }
                                                           player[row][line] =' ';
                                                           line-=1;
                                                           player[row][line] ='@';}
                                                   break;


                                        case 'w' : {
                                                           if(player[row-1][line] == '#')
                                                                   break;
                                                           if(player[row-1][line] == '$')
                                                           {
                                                                   if(player[row-2][line] == '#' || player[row-2][line] == '$')
                                                                           break;
                                                                   row -= 1;
                                                                   player[row+1][line] = ' ';
                                                                   player[row][line] = '@';
                                                                   player[row-1][line] = '$';
                                                                   break;
                                                           }
                                                           player[row][line] =' ';
                                                           row-=1;
                                                           player[row][line] ='@';}
                                                   break;
                                        case 's' : {
                                                           if(player[row+1][line] == '#')
                                                                   break;
                                                           if(player[row+1][line] == '$')
                                                           {
                                                                   if(player[row+2][line] == '#' || player[row+2][line] == '$')
                                                                           break;
                                                                   row += 1;
                                                                   player[row-1][line] = ' ';
                                                                   player[row][line] = '@';
                                                                   player[row+1][line] = '$';
                                                                   break;
                                                           }
                                                           player[row][line] =' ';
                                                           row+=1;
                                                           player[row][line] ='@';}
                                                   break;
                                        default : {
                                                          printf("\n다시 입력해 주세요\n");}
                                                  break;
                                }

                                system("clear");
                                
                                int n;               //중간에 O의 위치 확인
                                for(n=0;n<=Ocount;n++)
                                {
                                        if(player[0][O[n]] == ' ')
                                                player [0][O[n]] = 'O';
                                }


                                for (int i = 0; i<box_row*box_line-1 ;i++){
                                        if (player[0][i]==0)
                                                printf(" ");
                                        else
                                                printf("%c", player[0][i] );
                                }
                                printf("\n");
                        }
                }
                return 0;
        }
