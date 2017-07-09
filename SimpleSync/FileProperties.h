#pragma once



class FileProperties
{
public:
    FileProperties(const CString& fileName = CString(), BOOL isDir = FALSE);
    ~FileProperties();

    BOOL operator< (const FileProperties& file) const;
    BOOL operator== (const FileProperties& file) const;

    CString getFileName() const;
    CString getFullPath() const;
    CString getFileFolder() const;

    BOOL isDirectory() const;

private:
    CString m_fileName;

    BOOL m_isDirectory;
};

