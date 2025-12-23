#pragma once

#ifndef UMAMUSUMELOCALIFYANDROID_GAME_HPP
#define UMAMUSUMELOCALIFYANDROID_GAME_HPP

#include <string>
#include <unistd.h> // access 和 F_OK 需要这个头文件，原代码可能隐含包含但在某些环境需要显式包含

#define Unity2019 "2019.4.21f1"
#define Unity2020 "2020.3.24f1"

using namespace std;

namespace Game
{
    enum class Region
    {
        UNKNOWN,
        JAP,
        KOR,
        TWN,
        CHN, // 新增：中国大陆
    };

    enum class Store
    {
        Google,
        // Ex. OneStore, MyCard...
        Other
    };

    inline auto currentGameRegion = Region::UNKNOWN;
    inline auto currentGameStore = Store::Google;

    inline auto GamePackageName = "jp.co.cygames.umamusume"s;
    inline auto GamePackageNameKor = "com.kakaogames.umamusume"s;
    inline auto GamePackageNameTwnGoogle = "com.komoe.kmumamusumegp"s;
    inline auto GamePackageNameTwnMyCard = "com.komoe.kmumamusumemc"s;
    inline auto GamePackageNameChn = "com.bilibili.umamusu"s; // 新增：国服包名

    static bool IsPackageNameEqualsByGameRegion(const char *pkgNm, Region gameRegion)
    {
        string pkgNmStr = string(pkgNm);
        if (pkgNmStr.empty())
        {
            return false;
        }
        switch (gameRegion)
        {
        case Region::JAP:
            if (pkgNmStr == GamePackageName)
            {
                currentGameRegion = Region::JAP;
                currentGameStore = Store::Google;
                return true;
            }
            break;
        case Region::KOR:
            if (pkgNmStr == GamePackageNameKor)
            {
                currentGameRegion = Region::KOR;
                currentGameStore = Store::Google;
                return true;
            }
            break;
        case Region::TWN:
            if (pkgNmStr == GamePackageNameTwnGoogle)
            {
                currentGameRegion = Region::TWN;
                currentGameStore = Store::Google;
                return true;
            }
            else if (pkgNmStr == GamePackageNameTwnMyCard)
            {
                currentGameRegion = Region::TWN;
                currentGameStore = Store::Other;
                return true;
            }
            break;
        case Region::CHN: // 新增：国服判断逻辑
            if (pkgNmStr == GamePackageNameChn)
            {
                currentGameRegion = Region::CHN;
                // 国服通常也是单一渠道（Bilibili），暂时归类为 Default/Google 类型处理
                currentGameStore = Store::Google;
                return true;
            }
            break;
        case Region::UNKNOWN:
        default:
            break;
        }
        return false;
    }

    static string GetPackageNameByGameRegionAndGameStore(Region gameRegion, Store gameStore)
    {
        if (gameRegion == Region::JAP)
            return GamePackageName;
        if (gameRegion == Region::KOR)
            return GamePackageNameKor;
        if (gameRegion == Region::TWN)
        {
            if (gameStore == Store::Other)
            {
                return GamePackageNameTwnMyCard;
            }
            return GamePackageNameTwnGoogle;
        }
        if (gameRegion == Region::CHN)
        {
            return GamePackageNameChn;
        }
        return "";
    }

    static string GetCurrentPackageName()
    {
        return GetPackageNameByGameRegionAndGameStore(currentGameRegion, currentGameStore);
    }

    static Region CheckPackageNameByDataPath()
    {
        // Check JAP
        if (access(
                "/data/data/"s
                    .append(GetPackageNameByGameRegionAndGameStore(Region::JAP,
                                                                   Store::Google))
                    .append(
                        "/cache")
                    .data(),
                F_OK) == 0)
        {
            return Region::JAP;
        }
        // Check KOR
        if (access(
                "/data/data/"s
                    .append(GetPackageNameByGameRegionAndGameStore(Region::KOR,
                                                                   Store::Google))
                    .append(
                        "/cache")
                    .data(),
                F_OK) == 0)
        {
            return Region::KOR;
        }
        // Check TWN (Google)
        if (access(
                "/data/data/"s
                    .append(GetPackageNameByGameRegionAndGameStore(Region::TWN,
                                                                   Store::Google))
                    .append(
                        "/cache")
                    .data(),
                F_OK) == 0)
        {
            return Region::TWN;
        }
        // Check TWN (MyCard)
        if (access(
                "/data/data/"s
                    .append(GetPackageNameByGameRegionAndGameStore(Region::TWN,
                                                                   Store::Other))
                    .append(
                        "/cache")
                    .data(),
                F_OK) == 0)
        {
            currentGameStore = Store::Other;
            return Region::TWN;
        }

        if (access(
                "/data/data/"s
                    .append(GetPackageNameByGameRegionAndGameStore(Region::CHN,
                                                                   Store::Google))
                    .append(
                        "/cache")
                    .data(),
                F_OK) == 0)
        {
            return Region::CHN;
        }
        return Region::UNKNOWN;
    }
}

#endif