# File-allocation-table
In this section, we will deploy a virtual filesystem. Most filesystems have multiple levels of directory structures, but for this command, we'll use only one level. These files are stored on a virtual drive in the Raspberry's memory. The storage location of files is managed in the File Allocation Table (FAT). The following functionality must be realized:

- write to a file \n
- read from a file \n
- delete a file
-
• copy a file
• rename a file
• print a list of all files and their sizes
• display the maximum size of a file to save
• rearrange the location of the files on the virtual disk so that all empty space is concatenated
