#include "stdafx.h"
#include "EmptyOperation.h"



EmptyOperation::EmptyOperation(const FileProperties& file, const FileProperties& equalFile)
    : SyncOperation(SyncOperation::TYPE::EMPTY, file), m_equalFile(equalFile)
{
}

EmptyOperation::~EmptyOperation()
{
}



BOOL EmptyOperation::execute()
{
    return TRUE;
}

BOOL EmptyOperation::affectsFile(const FileProperties& file) const
{
    auto thisFile = getFile();
    auto equalFile = getEqualFile();

    if (thisFile == file || equalFile == file)
        return TRUE;

    if (thisFile.isFolder())
    {
        BOOL thisIsParentFolder = file.isParentFolder(thisFile);
        BOOL equalIsParentFolder = file.isParentFolder(equalFile);

        return thisIsParentFolder || equalIsParentFolder;
    }

    return FALSE;
}

BOOL EmptyOperation::dependsOn(const SyncOperation* operation) const
{
    BOOL affectsFile = operation->affectsFile(getFile());
    BOOL affectsEqualFile = operation->affectsFile(getEqualFile());

    return affectsFile || affectsEqualFile;
}

FileProperties EmptyOperation::getEqualFile() const
{
    return m_equalFile;
}
