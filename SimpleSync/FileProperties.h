#pragma once



class FileProperties
{
public:
    FileProperties(const CString& fileName = CString());
    ~FileProperties();

    BOOL operator< (const FileProperties& file) const;
    BOOL operator== (const FileProperties& file) const;

    CString getFileName() const;
    CString getFullPath() const;
    CString getFileFolder() const;

private:
    CString m_fileName;
};

