 

/*
********* stores num as char in argument "c" **********
*@param c "the array where to write"
*@param num "the number to be converted"
*@param c_len "length of array pointed by c"
*/
void num2arr( char* c, int num, int c_len) {
    for(int i=0; i<c_len; i++) {
        c[c_len- i- 1]= num%10 + 48;
        num/= 10;
    }
}

/*
********* return starting position of a "roll no" **********
*************** in the file stored as heap  ****************
*@param index "index to be cnverted"
*@param roll_len "length of a roll no"
*/
int fileIndexOf(int index) {
    int res= ROLL_COUNT_SIZE+1;
    res+= (ROLL_LEN+1)*(index+1);
    return res;
}