# SConstruct file. Used by the build application SCons to build all homework projects in this directory.
# Invoke SCons by using the command "scons".
#
# Author: Christopher Sheaf
# Last Updated: 10/26/14

import os
import csv
import re
from zipfile import ZipFile


# Define utility methods.
def create_directories(id_list):
    """Creates a directory for every entry in id_list."""
    for entry in id_list:
        if not os.path.exists(entry):
            os.mkdir(entry)


def read_csv_file(filename):
    with open(filename, 'rb') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=',')
        id_list = []

        for row in csv_reader:
            id_list.extend(row)
        return id_list


def get_source_files():
    for dir_name, subdir_list, file_list in os.walk('.'):
        source_files = []
        for filename in file_list:
            if re.match('^.+\.c$', filename):  # If this is a 'c' source file:
                source_files.append(filename)
        if len(source_files) > 0:
            yield dir_name, source_files


def unpack_zip_file(filename):
    with ZipFile(filename, 'r') as archive:
        for compressed_file in archive.infolist():
            print compressed_file.filename


# Begin SConstruct Script.
# Get invocation arguments.
zip_filename = ARGUMENTS.get('zip', None)
csv_filename = ARGUMENTS.get('csv', None)

if csv_filename and os.path.exists(csv_filename):
    student_ids = read_csv_file(csv_filename)
    create_directories(student_ids)

if zip_filename:
    print 'Unpacking zip file...'
    unpack_zip_file(zip_filename)

for directory, files in get_source_files():
    Program(target=directory + '/a', source=files)
