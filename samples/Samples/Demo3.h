// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo3
	: public Scene
{
	MusicPtr music;			// ���ֶ���
	TextPtr volume_text;	// ��������
	TextPtr state_text;		// ����״̬����

public:
	static ScenePtr Create()
	{
		return new Demo3;
	}

	Demo3()
	{
		// ��������
		music = new Music;
		if (!music->Load(L"res/splash.mp3"))
		{
			music = nullptr;

			TextPtr err = new Text(L"��Ƶ�ļ�����ʧ��");
			this->AddChild(err);
		}

		// �������֣������������ò���ѭ��������-1 ��ʾѭ�����ţ�
		music->Play(-1);

		// ����˵������
		TextPtr intro_text = new Text(L"�����¼���������\n���ո����ͣ�����");
		intro_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 50);

		// ������������
		volume_text = new Text(L"��ǰ������");
		volume_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 30);

		// ����״̬����
		state_text = new Text(L"��ǰ״̬��");
		state_text->SetPosition(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 60);

		// ���ӵ�����
		this->AddChild(intro_text);
		this->AddChild(volume_text);
		this->AddChild(state_text);
	}

	void OnUpdate(Duration dt) override
	{
		if (music == nullptr)
			return;

		// ��ȡ�����Ͳ���״̬
		float volume = music->GetVolume();
		bool playing = music->IsPlaying();

		// �޸��ı�
		volume_text->SetText(L"��ǰ������" + std::to_wstring(volume));
		state_text->SetText(playing ? L"��ǰ״̬�����ڲ���" : L"��ǰ״̬��ֹͣ����");

		// ��ȡ�����豸
		auto& input = Input::Instance();

		// ���ո����ͣ�����
		if (input.WasPressed(KeyCode::Space))
		{
			music->IsPlaying() ? music->Pause() : music->Resume();
		}

		// �����¼���������
		if (input.WasPressed(KeyCode::Up))
		{
			music->SetVolume(volume + 0.1f);
		}
		else if (input.WasPressed(KeyCode::Down))
		{
			music->SetVolume(volume - 0.1f);
		}
	}
};