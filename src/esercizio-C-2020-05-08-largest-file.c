#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

unsigned long get_file_size(char * fname);
int there_are_directory(char * file_name);
char * find_largest_file(char * directory_name, int explore_subdirectories_recursively, int * largest_file_size);
// https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html


int main(int argc, char *argv[]) {
	int largest_file_size = 0;
	char * largest_file;
	char * fileName;

	fileName = (argc == 1) ? "." : argv[1];

	errno = 0;

	largest_file = find_largest_file(fileName, there_are_directory(fileName), &largest_file_size);

	if (largest_file == NULL)
		printf("Non sono presenti file regolari nella directory %s\n", fileName);
	else
		printf("File regolare più grande:\n%s | dimensione = %d\n", largest_file, largest_file_size);

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
	DIR * dir_stream_ptr;
	struct dirent *ep;

	char * largest_file;
	int file_size;

	dir_stream_ptr = opendir(directory_name);

	if (dir_stream_ptr == NULL) {
		printf("cannot open directory %s! bye", directory_name);
		return NULL;
	}

	printf("there are dir: %d\nlargest_file_size : %d\n", explore_subdirectories_recursively, *largest_file_size);

	if (explore_subdirectories_recursively == 0) {

		while ((ep = readdir(dir_stream_ptr)) != NULL) {
			if (ep->d_type == DT_REG) {
				printf("%s\n", ep->d_name);
				file_size = get_file_size(ep->d_name);
				if (*largest_file_size < file_size) {
					*largest_file_size = file_size;
					largest_file = ep->d_name;
				}

			}
		}

		if (errno) {
			perror("readdir() error");
			return NULL;
		}
	} else {

		while ((ep = readdir(dir_stream_ptr)) != NULL) {
			if (ep->d_type == DT_REG) {
				printf("%s\n", ep->d_name);
				file_size = get_file_size(ep->d_name);

				if (*largest_file_size < file_size) {
					*largest_file_size = file_size;
					largest_file = ep->d_name;
				}
			} else if (ep->d_type == DT_DIR) {
				printf("%s\n", ep->d_name);
				largest_file = find_largest_file(ep->d_name, there_are_directory(ep->d_name), largest_file_size);
			}
		}

		if (errno) {
			perror("readdir() error");
			return NULL;
		}
	}

	closedir(dir_stream_ptr);
	char * cwd = getcwd(NULL, 0);
	largest_file = strcat(cwd, largest_file);

	return largest_file;
}


