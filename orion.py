# Libraries Import
import shutil
import sys
import os
import argparse

# Define all the program's arguments
def GenerateParser():
    parser = argparse.ArgumentParser(description="File Management")
    
    subparsers = parser.add_subparsers(dest="command", help="Available commands")

    # Subparser for the copy command
    copy_parser = subparsers.add_parser("copy", help="Copy a file into a new location")
    copy_parser.add_argument("--from", dest="sourceFile", required=True, help="Path to the source file")
    copy_parser.add_argument("--to", dest="destinationPath", required=True, help="Path to the destination folder")

    # Subparser for the move command
    move_parser = subparsers.add_parser("move", help="Move a file into a new location")
    move_parser.add_argument("--from", dest="sourceFile", required=True, help="Path to the source file")
    move_parser.add_argument("--to", dest="destinationPath", required=True, help="Path to the destination folder")

    # Return the generated parser
    return parser

# Function to move a file from a source location to a destination location
def CopyFile(args):
    """ Move a file to a destination folder and remove it at the finish """
    MoveFile(args, False)

# Function to copy a file from a source location to a destination location
def MoveFile(args, removeSource):
    """ Copy a file to a destination folder """
    sourceFile = args.sourceFile
    destinationPath = args.destinationPath
    destinationFile = os.path.join(destinationPath, os.path.basename(sourceFile))

    totalSize = os.path.getsize(sourceFile)
    bytesTransferred = 0
    bufferSize = 1024

    # If the destination folder doesn't exist it will create it
    os.makedirs(destinationPath, exist_ok=True)

    # Write the file into the new location
    with open(sourceFile, 'rb') as source:
        with open(destinationFile, 'wb') as destination:
            while True:
                buffer = source.read(bufferSize)
                if not buffer:
                    break
                destination.write(buffer)
                bytes_transferred += len(buffer)
                progress = (bytesTransferred / totalSize) * 100
                print(f"Moving: {progress:.2f}% completed", end='\r')
    
    # Remove the source file only if the dedicated flag is setted to true
    if removeSource == True:
            os.remove(sourceFile)

# Main Function of the program
if __name__ == "__main__":

    # Generate the new parser
    parser = GenerateParser()

    # Retrieve all the arguments using the parser
    args = parser.parse_args()

    if args.command == "copy":
        CopyFile(args)
    elif args.command == "move":
        MoveFile(args, True)
    else:
        parser.print_help()
