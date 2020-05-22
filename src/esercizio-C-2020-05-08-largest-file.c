#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

unsigned long get_file_size(char * fname);
int there_are_directory(char * file_name);
char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size);
// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html

int main(int argc, char *argv[]) {

	DIR * dir_stream_ptr;
	struct dirent *ep;

	char * fileName;
	unsigned long file_size = 0;
	unsigned long largest_file = 0;


	fileName = (argc == 1) ? "." : argv[1];

	errno = 0;

// man 3 opendir
//    DIR *opendir(const char *name);
//    DIR *fdopendir(int fd);


	dir_stream_ptr = opendir(fileName);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s! bye", fileName);

		return EXIT_FAILURE;
	}

	if (there_are_directory(fileName)) {
		// allora cerca nelle directory in modo ricorsivo
	} else {
		// cerca file regolare maggiore se c'è
	}

	while ((ep = readdir(dir_stream_ptr)) != NULL) {

		printf("%-10s ", (ep->d_type == DT_REG) ?  "regular" :
		                                    (ep->d_type == DT_DIR) ?  "directory" :
		                                    (ep->d_type == DT_FIFO) ? "FIFO" :
		                                    (ep->d_type == DT_SOCK) ? "socket" :
		                                    (ep->d_type == DT_LNK) ?  "symlink" :
		                                    (ep->d_type == DT_BLK) ?  "block dev" :
		                                    (ep->d_type == DT_CHR) ?  "char dev" : "???");

		if (ep->d_type == DT_REG) {
			file_size = get_file_size(ep->d_name);
		}

		printf("%s \t%lu\n", ep->d_name, file_size);

		// come trovo il file size? posso usare stat (man 2 stat)
	}

	if (errno) {
		perror("readdir() error");
	}

	closedir(dir_stream_ptr);

	puts("finished! bye!");

	return EXIT_SUCCESS;
}

unsigned long get_file_size(char * fname) {

	struct stat st;

	int res = stat(fname, &st);

	if (res == -1) {
		perror("stat error");
		return -1;
	} else
		return st.st_size;
}

int there_are_directory(char * file_name) {
	DIR * dir_stream_ptr;
	struct dirent *ep;

	dir_stream_ptr = opendir(file_name);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s! bye", file_name);
		return EXIT_FAILURE;
	}

	while ((ep = readdir(dir_stream_ptr)) != NULL) {
		if (ep->d_type == DT_DIR)
			return 1;
	}

	return 0;
}

char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size) {
	char * largest_file;


}


