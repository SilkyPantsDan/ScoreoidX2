/****************************************************************************
 Copyright (c) 2013      Edwin van Poeijer
 
 http://www.vanpoeijer.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __quadro__Scoreoid__
#define __quadro__Scoreoid__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "document.h"
#include "rapidjson.h"
#include "ScoreoidObjects.h"
#include "ScoreoidAPI.h"


using namespace cocos2d;
using namespace rapidjson;

/*
 * Delegate class for Scoreoid Wrapper
 *
 * The callbacks return a CCDictionary with the following specifications
 *
 * KEY: api     (CCString* api call description; for example: getScores
 * KEY: status  (CCInteger*  with Status)
 * KEY: message (CCString* with status message, for example: parse error, or success
 * KEY: result  (CCArray* with all the results. Results are CCDictionary* objects
 *
 *
 * The ScoreoidAvailable callback is called when login was succesfull.
 * it returns the player information as CCDictionary.
 */
class ScoreoidDelegate {
public:
    virtual ~ScoreoidDelegate() {}
    
    virtual void scoreCallback(SOScore* score,SOResult result) {}
    virtual void scoresCallback(CCArray* scores,SOResult result) {}
    
    virtual void playerCallback(SOPlayer* player,SOResult result){}
    virtual void playersCallback(CCArray* players,SOResult result){}
    
    virtual void gameCallback(SOGame* game,SOResult result) {}
    virtual void notificationsCallback(CCArray* notifications,SOResult result) {}
    
    virtual void scoreoidAvailable(SOGame* game,SOResult result) {}
};

/*
 * Scoreoid class
 *
 */
class Scoreoid : public CCObject
{
    
private:
    
    ScoreoidDelegate* m_delegate;
    
    /*
     * Scoreoid Game and API keys
     */
    std::string _gameID;
    std::string _ApiKey;
    std::string _apiCallString;
    /*
     *  running actions
     */
    bool _scoreoidAvailable;
    bool _actionRunning;
    int  _currentApiCall;
    
    /*
     * Local user
     */
    std::string _localUserId;
    bool _createUser;
    bool _localUserLoggedIn;
    SOPlayer* _localPlayer;
    
    // Private constructor
	/** allocates and initializes a object.
     The node will be created as "autorelease".
     */
    static Scoreoid * create(void);
    
    /*
     * Private constructor
     */
    Scoreoid();
    
public:
    /*
     * Static
     */
    static Scoreoid* instance;
    static Scoreoid* GetInstance();
    
    virtual ~Scoreoid(void);
    
    /*
     * Set the delegate
     */
    void setDelegate(ScoreoidDelegate* delegate) { m_delegate = delegate; }
    ScoreoidDelegate* getDelegate() { return m_delegate; }
    
    /*
     * Init Scoreoid
     */
    void initScoreoid(const char* gameID, const char* apiKey);
    
    /*
     * JSON Helpers
     */
    std::string getStringValue(const Value& member, const char* field);
    std::string getStringValue(Value::ConstMemberIterator itr,const char* field);
    bool getBoolValue(const Value& member, const char* field);
    int getIntValue(const Value& member, const char* field);
    double getDoubleValue(const Value& member, const char* field);
    
    /*
     * Helper methods
     */
    CCString* getStringJSON(Value::Member *iterator, const char* field);
    const char* removeEmptyFields(const char* value, std::string delimiter);
    
    
    /*
     * Methods
     */
    
    /*
     * Local user
     */
    void setLocalUserID(const char* localID) { this->_localUserId = localID;};
    const char* getLocalUserID() { return this->_localUserId.c_str();};
    bool localUserLoggedIn() { return this->_localUserLoggedIn;};
    
    // Http request and response handles
    bool HttpRequest(const char* apiUrl,const char* data, const char* tag,SEL_CallFuncND pSelector);
    
    void HttpRequestScoreCallback(cocos2d::CCNode *sender, void *data);
    void HttpRequestScoresCallback(cocos2d::CCNode *sender, void *data);
    void HttpRequestPlayerCallback(cocos2d::CCNode *sender, void *data);
    void HttpRequestPlayersCallback(cocos2d::CCNode *sender, void *data);
    void HttpRequestGameCallback(cocos2d::CCNode *sender, void *data);
    void HttpRequestNotificationsCallback(cocos2d::CCNode *sender, void *data);
    
    /*
     *   Gets notifications for this game
     */
    bool getNotification(); //method lets you pull your game’s in game notifications.
    //void handleGetNotification(const char* jsonData,int& result);
    /*
     gets the total for the following game field’s bonus, gold, money, kills, lifes, time_played and unlocked_levels.
     field => String Value, (bonus, gold, money, kills, lifes, time_played, unlocked_levels) [Required]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     */
    bool getGameTotal(const char* field, const char* start_date, const char* end_date, const char* platform);
    
    /*
     gets the lowest value for the following game field’s bonus, gold, money, kills, lifes, time_played and unlocked_levels.
     field => String Value, (bonus,best_score, gold, money, kills, lifes, time_played, unlocked_levels) [Required]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     */
    bool getGameLowest(const char* field, const char* start_date, const char* end_date, const char* platform);
    
    /*
     gets the top value for the following game field’s bonus, gold, money, kills, lifes, time_played and unlocked_levels.
     field => String Value, (bonus, best_score, gold, money, kills, lifes, time_played, unlocked_levels) [Required]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     */
    bool getGameTop(const char* field, const char* start_date, const char* end_date, const char* platform);
    
    /*
     gets the average for the following game field’s bonus, gold, money, kills, lifes, time_played and unlocked_levels.
     field => String Value, (bonus, best_score, gold, money, kills, lifes, time_played, unlocked_levels) [Required]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     */
    bool getGameAverage(const char* field, const char* start_date, const char* end_date, const char* platform);
    
    /*
     API method lets you get all the players for a specif game.
     order_by => String Value, (date or score) [Optional]
     order => String Value, asc or desc [Optional]
     limit => Number Value, the limit, "20" retrieves rows 1 - 20 | "10,20" retrieves 20 scores starting from the 10th [Optional]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => String Value, needs to match the string value that was used when creating the player [Optional]
     */
    bool getPlayers(const char* order_by, const char* order, const char* limit, const char* start_date, const char* end_date, const char* platform);
    
    /*
     method lets you pull a specific field from your game info.
     field => String Value, (name, short_description, description, game_type, version, levels, platform, play_url, website_url,
     created, updated, player_count, scores_count, locked, status) [Required]
     */
    bool getGameField(const char* field);
    
    /*
     method lets you pull all your game information.
     */
    bool getGame();
    
    /*
     method lets you pull a specific field from your player info.
     username => String Value, the players username [Required]
     field => String Value, the selected field (username, password, unique_id, first_name, last_name, email, platform, created,
     updated, bonus, achievements, best_score, gold, money, kills, lives, time_played, unlocked_levels, unlocked_items, inventory,
     last_level, current_level, current_time, current_bonus, current_kills, current_achievements, current_gold, current_unlocked_levels,
     current_unlocked_items, current_lifes, xp, energy, boost, latitude, longitude, game_state, platform, rank) [Required]
     */
    bool getPlayerField(const char* username,const char* field);
    
    /*
     method lets you pull all the scores for a player.
     username => String Value, the players username [Required]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool getPlayerScores(const char* username, const char* start_date, const char* end_date, const char* difficulty);
    
    /*
     method deletes a player and all the players scores.
     username => String Value, the players username [Required]
     */
    bool deletePlayer(const char* username);
    
    /*
     method Check if player exists and returns the player information. Post parameters work as query conditions. This method can be used for login by adding username and password parameters.
     id =>  The players ID [String] [Optional]
     username => String Value, the players username [Required]
     password => The players password [String] [Optional]
     email => The players email [String] [Optional]
     */
    bool getPlayer(const char* username, const char* id, const char* password, const char* email);
    
    /*
     method lets you edit your player information.
     username => String Value, the players username [Required]
     fields = string with all fields in format password=[value] separated by &
     password, unique_id, first_name, last_name, email, created, updated, bonus, achievements, gold, money, kills, lives, time_played, unlocked_levels, unlocked_items, inventory, last_level, current_level, current_time, current_bonus, current_kills, current_achievements, current_gold, current_unlocked_levels, current_unlocked_items, current_lifes, xp, energy, boost, latitude, longitude, game_state, platform)
     For description of all the fields www.scoreoid.com
     */
    bool editPlayer(const char* username, const char* fields);
    
    /*
     method lets you count all your players.
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     */
    bool countPlayers(const char* start_date, const char*end_date, const char* platform);
    
    /*
     method lets you update your player field’s.
     username => String Value, the players username [Required]
     field => String Value, (password, unique_id, first_name, last_name, email, created, updated, bonus, achievements, gold, money, kills, lives, time_played, unlocked_levels, unlocked_items, inventory, last_level, current_level, current_time, current_bonus, current_kills, current_achievements, current_gold, current_unlocked_levels, current_unlocked_items, current_lifes, xp, energy, boost, latitude, longitude, game_state, platform) [Required]
     value => String Value, the field value [Required]
     */
    bool updatePlayerField(const char* username, const char* field, const char* value);
    
    /*
     method lets you create a player with a number of optional values.
     username => The players username [String] [Required]
     fields = string with all fields in format password=[value] separated by &
     password, unique_id, first_name, last_name, email, created, updated, bonus, achievements, gold, money, kills, lives, time_played, unlocked_levels, unlocked_items, inventory, last_level, current_level, current_time, current_bonus, current_kills, current_achievements, current_gold, current_unlocked_levels, current_unlocked_items, current_lifes, xp, energy, boost, latitude, longitude, game_state, platform)
     */
    bool createPlayer(const char* username, const char* fields);
    
    /*
     method lets you count all your game best scores.
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool countBestScores(const char* start_date, const char*end_date, const char* platform, const char* difficulty);
    
    /*
     method lets you get all your game average scores.
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool getAverageScore(const char* start_date, const char*end_date, const char* platform, const char* difficulty);
    
    /*
     method lets you get all your games best scores.
     order_by => String Value, (date or score) [Optional]
     order => String Value, asc or desc [Optional]
     limit => Number Value, the limit, "20" retrieves rows 1 - 20 | "10,20" retrieves 20 scores starting from the 10th [Optional]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]     */
    bool getBestScores(const char* order_by, const char* order, const char* limit,const char* start_date, const char*end_date, const char* platform, const char* difficulty);
    
    /*
     method lets you count all your game scores.
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool countScores(const char* start_date, const char*end_date, const char* platform, const char* difficulty);
    
    /*
     method lets you pull all your game scores.
     order_by => String Value, (date or score) [Optional]
     order => String Value, asc or desc [Optional]
     limit => Number Value, the limit, "20" retrieves rows 1 - 20 | "10,20" retrieves 20 scores starting from the 10th [Optional]
     start_date => String Value, YYY-MM-DD format [Optional]
     end_date => String Value, YYY-MM-DD format [Optional]
     platform => The players platform needs to match the string value that was used when creating the player  [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool getScores(const char* order_by, const char* order, const char* limit,const char* start_date, const char*end_date, const char* platform, const char* difficulty);
    
    /*
     method lets you create a user score.
     username => String Value, if user does not exist it well be created [Required]
     score => Integer Value, [Required]
     platform => String Value, [Optional]
     unique_id => Integer Value, [Optional]
     difficulty => Integer Value, between 1 to 10 (don't use 0 as it's the default value) [Optional]
     */
    bool createScore(const char* username, const char* score, const char* platform, const char* unique_id, const char* difficulty);
    
    /*
     * Login using an ID, if needed the user can be created; returns the player description
     * set createPlayer to TRUE if you want to create the player when the player is unknown.
     */
    bool login(const char* playerID, const char* firstName, const char* lastName,bool shoudCreate);
    
    /*
     * Local login handlers
     */
    void loginPlayerHandler(SOPlayer* player,SOResult result);
    void loginCreatePlayerHandler(SOPlayer* player,SOResult result);
    
};


#endif /* defined(__quadro__Scoreoid__) */
