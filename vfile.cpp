#include "vfile.h"

void File_process::Vfile::insert_to_llist(Vfile *file, Vfile *front, Vfile *behind)
{
    if (front) {
        file->prev = front;
        file->next = front->next;
        front->next = file;
    } else if (behind) {
        file->next = behind;
        file->prev = behind->prev;
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
