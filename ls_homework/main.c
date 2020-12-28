#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void myls(struct dirent * dir) {
	char * path;
	path = calloc(sizeof(char) * (strlen(dir->d_name)) + 2, sizeof(char));
	path[0] = '.';
	path[1] = '/';
	strcpy(&(path[2]), dir->d_name);


	struct stat S;
	memset(&S, 0, sizeof(struct stat));
	stat(path, &S);

	printf((S.st_mode & S_IFDIR) ? "d" : "-");

	printf((S.st_mode & S_IRUSR) ? "r" : "-");
	printf((S.st_mode & S_IWUSR) ? "w" : "-");
	printf((S.st_mode & S_IXUSR) ? "x" : "-");

	printf((S.st_mode & S_IRGRP) ? "r" : "-");
	printf((S.st_mode & S_IWGRP) ? "w" : "-");
	printf((S.st_mode & S_IXGRP) ? "x" : "-");

	printf((S.st_mode & S_IROTH) ? "r" : "-");
	printf((S.st_mode & S_IWOTH) ? "w" : "-");
	printf((S.st_mode & S_IXOTH) ? "x" : "-");


	printf(" %2lu", S.st_nlink);

	struct passwd * pwd;
	pwd = getpwuid(S.st_uid);
	printf(" %s", pwd->pw_name);

	struct group * grp;
	grp = getgrgid(S.st_gid);
	printf(" %s", grp->gr_name);

	printf(" %5d ", (int)S.st_size);

	struct tm * time_ptr;
	time_t It = S.st_ctime;
	time_ptr = localtime(&It);
	printf(" %02d.%02d", time_ptr->tm_mday, time_ptr->tm_mon+1);
	printf(" %02d:%02d ", time_ptr->tm_hour, time_ptr->tm_min);

	printf("%s\n", dir->d_name);
}

int memory_blocks(char *dirname) {
    register struct dirent *dir_buf;
    DIR *fdir;
    int result = 0;

    struct stat S;
    if ((fdir = opendir(dirname)) == NULL) {
        perror("Can't read directory\n");
        return -1;
    }

    while ((dir_buf = readdir(fdir)) != NULL) {
       	char file_path[strlen(dirname) + strlen(dir_buf->d_name) + 10];
        sprintf(file_path, "%s/%s", dirname, dir_buf->d_name);
        stat(file_path, &S);
        result += S.st_blocks;
    }
    closedir(fdir);

    return result / 2;
}

int main() {
	int n;
	struct dirent ** entry;
	if((n = scandir("./", &entry, NULL, alphasort)) == 1)
		return -1;	
	int dir_size = n-1;

	printf("total %d\n", memory_blocks("."));
	while(n--) {
		myls(entry[dir_size - n]);
	}
	return 0;
}
