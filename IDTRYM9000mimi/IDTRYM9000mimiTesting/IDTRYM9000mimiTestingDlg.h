
// IDTRYM9000mimiTestingDlg.h : ͷ�ļ�
//

#pragma once


// CIDTRYM9000mimiTestingDlg �Ի���
class CIDTRYM9000mimiTestingDlg : public CDialogEx
{
// ����
public:
	CIDTRYM9000mimiTestingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IDTRYM9000MIMITESTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtOpen();
	afx_msg void OnBnClickedBtRfturnon();
	afx_msg void OnBnClickedBtRftrunoff();
	afx_msg void OnBnClickedBtAuthentication();
	afx_msg void OnBnClickedBtGetdata();
	CString m_csResults;
	afx_msg void OnBnClickedBtCrtGetstatus();
	afx_msg void OnBnClickedBtActiveic();
	afx_msg void OnBnClickedBtDeactivateic();
	afx_msg void OnBnClickedBtActivecontactlessic();
	afx_msg void OnBnClickedBtDeactivatecontactlessic();
	afx_msg void OnBnClickedBtWarmreset();
	afx_msg void OnBnClickedBtIccommand();
	afx_msg void OnBnClickedBtRfcommand();
	afx_msg void OnBnClickedBtGetdevcategory();
	afx_msg void OnBnClickedBtReset();
	afx_msg void OnBnClickedBtDevclose();
	afx_msg void OnBnClickedBtGetlasterr();
	afx_msg void OnBnClickedBtCrtopen();
	int m_nPort;
	afx_msg void OnBnClickedBtCreate();
	afx_msg void OnBnClickedBtRelease();
};
