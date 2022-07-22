#include "tinbes.h"

#define TABLE_SIZE 25

// File allocation table: arrays of names, starting addresses and file lengths
string name[TABLE_SIZE];
long addr[TABLE_SIZE];
long size[TABLE_SIZE];
int noOfFiles = 0;

// Sort the file allocation table by starting address using bubble sort
void sort() {
  bool sorted = false;
  long tmp;
  string s;
  while (!sorted) {
    sorted = true;
    for (int i = 0; i < noOfFiles - 1; i++) {
      if (addr[i] > addr[i + 1]) { // in the wrong order?
        // swap two entries
        tmp = addr[i];
        addr[i] = addr[i + 1];
        addr[i + 1] = tmp;
        s = name[i];
        name[i] = name[i + 1];
        name[i + 1] = s;
        tmp = size[i];
        size[i] = size[i + 1];
        size[i + 1] = tmp;
        sorted = false;
      }
    }
  }
}

// Look for an unused block of disk space of at least fileSize and return starting address
long findFreeSpace(long fileSize) {
  sort();
  long prev = 0;
  for (int i = 0; i < noOfFiles; i++) {
    if (addr[i] - prev > fileSize) return prev;
    prev = addr[i] + size[i];
  }
  if (getSize() - prev > fileSize) return prev;
  return -1; // not found
}

// Return index in FAT of file with fileName
int findFile(string fileName) {
  for (int i = 0; i < noOfFiles; i++) {
    if (name[i] == fileName) return i;
  }
  return -1; // not found
}

void store(long fileSize, string fileName, char *data) {
  int f = findFreeSpace(fileSize);
  if (f == -1 || noOfFiles == TABLE_SIZE) {
    cout << "Not enough space." << endl;    
  }
  else if (findFile(fileName) != -1) {
    cout << "File already exists." << endl;
  }
  else { // create new entry in FAT


      addr[noOfFiles] = f;
      size[noOfFiles] = fileSize;
      name[noOfFiles] = fileName;
      noOfFiles++;
      sort();
      cout << "File succesfully  stored \n";
      
     

    for (int i = 0; i < fileSize; i++) {
      writeByte(data[i], f + i);
    }
  }
}

string retrieve(string fileName) {
  int f = findFile(fileName);
  if (f == -1) {
    cout << "File not found." << endl;
    return string();
  }
  string ret;
  for (long a = addr[f]; a < addr[f] + size[f]; a++) {
    
    
      ret += (char)readByte(a);
      
      

  }
  return ret;
}

void erase(string fileName) {
  int f = findFile(fileName);
  if (f == -1) {
    cout << "File not found." << endl;
    return;
  }
  noOfFiles--;
  // remove entry from FAT
  for (int i = f; i < noOfFiles; i++) {
    addr[i] = addr[i + 1];
    size[i] = size[i + 1];
    name[i] = name[i + 1];
  }
}

void copy(string fileName1, string fileName2) {
  string contents = retrieve(fileName1);
  long fileSize = contents.length();
  if (fileSize > 0) {
    char *data = (char *)malloc(fileSize);
    contents.copy(data, fileSize);
    store(fileSize, fileName2, data);
    free(data);
  }
}

void rename(string fileName1, string fileName2) {
  int f = findFile(fileName1);
  if (f == -1) {
    cout << "File not found." << endl;
  }
  else if (findFile(fileName2) != -1) {
    cout << "File already exists." << endl;
  }
  else { // change entry in FAT
    
    
      name[f] = fileName2;
      cout << "File succesfully renamed  \n";
     

  }
}

void files() {
  for (int i = 0; i < noOfFiles; i++) {
    cout << name[i] << '\t' << size[i] << endl;
  }
}

// Return largest unused block of disk space
long freeSpace() {
  sort();
  long largest = 0, previous = 0, length;
  for (int i = 0; i < noOfFiles; i++) { // find space between previous and start of file i
    
   
      length = addr[i] - previous;
      

    if (length > largest) largest = length;
    previous = addr[i] + size[i];
  }
  length = getSize() - previous;
  if (length > largest) largest = length;
  return largest;
}

void pack() {
  sort();
  long pos = 0, newpos;
  for (int i = 0; i < noOfFiles; i++) {
    newpos = pos;    
    for (long a = addr[i]; a < addr[i] + size[i]; a++) { // move data from a to pos
      
    
        addr[i+1] = a;
      
      
      pos++;
    }
    addr[i] = newpos; // update FAT
  }
}

int main() {
  string command;
  while (true) {
    command = readLine();
    if (command == "STORE") {
      string fileName = readLine();
      long fileSize = stoi(readLine());
      char* data = (char *)malloc(fileSize);
      cin.read(data, fileSize);
      store(fileSize, fileName, data);
      free(data);
    }
    if (command == "RETRIEVE") {
      string fileName = readLine();
      string data = retrieve(fileName);
      if (data.length() > 0) cout << data << endl;
    }
    if (command == "ERASE") {
      string fileName = readLine();
      erase(fileName);
    }
    if (command == "COPY") {
      string fileName1 = readLine();
      string fileName2 = readLine();
      copy(fileName1, fileName2);
    }
    if (command == "RENAME") {
      string fileName1 = readLine();
      string fileName2 = readLine();
      rename(fileName1, fileName2);
    }
    if (command == "FILES") {
      files();
    }
    if (command == "FREESPACE") {
      cout << freeSpace() << endl;
    }
    if (command == "PACK") {
      pack();
    }
  }
}

