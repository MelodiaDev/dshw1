initialize(char *file)

retrieve(int x,int y,int h,int w,vector<string> &res)

go_y(int y,int dy,int &resdy)
	//从第y列开始行内右移dy列（负数表示左移），实际移动的列数，存放在resdy
go_x(int dx,int y,int &resdx,int &resy)
	//从当前行下移dx行（负数表示上移），且到达第y列，实际到达的位置，实际移动的行数在resdx，实际的列数在resy
