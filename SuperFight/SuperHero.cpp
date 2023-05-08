void SuperHero::setNickname(const MyString& nickname)
{
	validation::isNicknameValid(nickname);
	this->nickname = nickname;
}
