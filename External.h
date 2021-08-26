#pragma once

class Request
{};

// возвращает nullptr если нужно завершить процесс, либо указатель на память,
// которую в дальнейшем требуется удалить
Request* GetRequest() throw();

// обрабатывает запрос, но память не удаляет
void ProcessRequest(Request* request) throw();

