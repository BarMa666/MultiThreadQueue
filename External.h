#pragma once

class Request
{};

// ���������� nullptr ���� ����� ��������� �������, ���� ��������� �� ������,
// ������� � ���������� ��������� �������
Request* GetRequest() throw();

// ������������ ������, �� ������ �� �������
void ProcessRequest(Request* request) throw();

