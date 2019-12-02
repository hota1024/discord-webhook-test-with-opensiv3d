# include <curl/curl.h>
# include <Siv3D.hpp> // OpenSiv3D v0.4.1

namespace Discord
{
	namespace Webhook
	{
		class WebhookClient
		{
		private:
			// Webhook URL.
			String url;

			// CURL.
			CURL *curl;

			// Headers.
			struct curl_slist *headers = NULL;

			// Initialize curl and headers.
			void initializeCurl()
			{
				curl = curl_easy_init();
				headers = curl_slist_append(headers, "Content-Type: application/json; charset=utf-8");

				curl_easy_setopt(curl, CURLOPT_URL, url.narrow().c_str());
				curl_easy_setopt(curl, CURLOPT_HEADER, headers);
			}
		public:
			WebhookClient() = default;

			WebhookClient(String url)
			{
			    this->url = url;
				initializeCurl();
			}

			~WebhookClient()
			{
				curl_easy_cleanup(curl);
			}

			// Set webhook url.
			void setUrl(String url)
			{
				this->url = url;
			}

			// Send text content.
			void sendContent(String content)
			{
				String data = U"";
				data += U"content=" + content;

				curl_easy_setopt(curl, CURLOPT_POST, 1);
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.narrow().c_str());
				curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
				CURLcode code = curl_easy_perform(curl);
			}
		};
	}
}

void Main()
{
	// ウィンドウの設定
	Window::Resize(500, 400);
	Window::SetTitle(U"Discord Webhook client test");
	Scene::SetBackground(Color(U"#7289DA"));

	// クライアントの生成
	String url = U"チャンネル→設定→Webhook→URL";
	Discord::Webhook::WebhookClient client(url);

	const Font contentFont(32);
	TextBox contentInput(contentFont, Window::ClientCenter().movedBy(-200, -50), 400);

	while (System::Update())
	{
        contentInput.update();
        if (SimpleGUI::ButtonAt(U"Send", Window::ClientCenter().movedBy(0, 50), 150))
		{
            String content = contentInput.getText();
            client.sendContent(content);
		}

		contentInput.draw();
		contentInput.drawOverlay();
	}
}
