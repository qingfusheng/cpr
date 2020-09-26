#ifndef CPR_SESSION_H
#define CPR_SESSION_H

#include <cstdint>
#include <fstream>
#include <memory>

#include "auth.h"
#include "body.h"
#include "connect_timeout.h"
#include "cookies.h"
#include "cprtypes.h"
#include "digest.h"
#include "limit_rate.h"
#include "low_speed.h"
#include "max_redirects.h"
#include "multipart.h"
#include "ntlm.h"
#include "parameters.h"
#include "payload.h"
#include "proxies.h"
#include "response.h"
#include "ssl_options.h"
#include "timeout.h"
#include "unix_socket.h"
#include "user_agent.h"
#include "verbose.h"

namespace cpr {

class Session {
  public:
    Session();
    ~Session();

    Session(Session&& other);
    Session& operator=(Session&& other);

    void SetUrl(const Url& url);
    void SetParameters(const Parameters& parameters);
    void SetParameters(Parameters&& parameters);
    void SetHeader(const Header& header);
    void SetTimeout(const Timeout& timeout);
    void SetConnectTimeout(const ConnectTimeout& timeout);
    void SetAuth(const Authentication& auth);
    void SetDigest(const Digest& auth);
    void SetUserAgent(const UserAgent& ua);
    void SetPayload(Payload&& payload);
    void SetPayload(const Payload& payload);
    void SetProxies(Proxies&& proxies);
    void SetProxies(const Proxies& proxies);
    void SetMultipart(Multipart&& multipart);
    void SetMultipart(const Multipart& multipart);
    void SetNTLM(const NTLM& auth);
    void SetRedirect(const bool& redirect);
    void SetMaxRedirects(const MaxRedirects& max_redirects);
    void SetCookies(const Cookies& cookies);
    void SetBody(Body&& body);
    void SetBody(const Body& body);
    void SetLowSpeed(const LowSpeed& low_speed);
    void SetVerifySsl(const VerifySsl& verify);
    void SetUnixSocket(const UnixSocket& unix_socket);
    void SetSslOptions(const SslOptions& options);
    void SetVerbose(const Verbose& verbose);

    // Used in templated functions
    void SetOption(const Url& url);
    void SetOption(const Parameters& parameters);
    void SetOption(Parameters&& parameters);
    void SetOption(const Header& header);
    void SetOption(const Timeout& timeout);
    void SetOption(const ConnectTimeout& timeout);
    void SetOption(const Authentication& auth);
    void SetOption(const Digest& auth);
    void SetOption(const UserAgent& ua);
    void SetOption(Payload&& payload);
    void SetOption(const Payload& payload);
    void SetOption(const LimitRate& limit_rate);
    void SetOption(Proxies&& proxies);
    void SetOption(const Proxies& proxies);
    void SetOption(Multipart&& multipart);
    void SetOption(const Multipart& multipart);
    void SetOption(const NTLM& auth);
    void SetOption(const bool& redirect);
    void SetOption(const MaxRedirects& max_redirects);
    void SetOption(const Cookies& cookies);
    void SetOption(Body&& body);
    void SetOption(const Body& body);
    void SetOption(const LowSpeed& low_speed);
    void SetOption(const VerifySsl& verify);
    void SetOption(const Verbose& verbose);
    void SetOption(const UnixSocket& unix_socket);
    void SetOption(const SslOptions& options);

    Response Delete();
    Response Download(std::ofstream& file);
    Response Get();
    Response Head();
    Response Options();
    Response Patch();
    Response Post();
    Response Put();

  private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace cpr

#endif
