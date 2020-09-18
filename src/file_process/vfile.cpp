#include "./vfile.h"

void File_process::Vfile::insert_to_llist(Vfile *file, Vfile *front, Vfile *behind)
{
    if (front && !behind) {
        file->prev = front;
        file->next = front->next;
        if (file->next) file->next->prev = file;
        front->next = file;
    } else if (behind && !front) {
        file->next = behind;
        file->prev = behind->prev;
        if (file->prev) file->prev->next = file;
        behind->prev = file;
    } else if (front && behind){
        file->prev = front;
        file->next = behind;
        front->next = file;
        behind->prev = file;
    } else {
        throw "Both parameters are nullptr.";
    }
}

File_process::Vfile *File_process::Vfile::remove_from_llist(File_process::Vfile *file)
{
    Vfile *front = file->prev;
    Vfile *behind = file->next;

    front->next = behind;
    behind->prev = front;

    return file;
}
