// JIS配列として認識されたキーボードをUS配列として使う & MacとWindowsのコンパチ

// macOSの設定でキーボードをJIS配列として認識させ、コントロールキーとコマンドキーを入れ替えておく
// Windowsの設定で「無変換」キーをIMEオフ（日本語入力オフ）に、「変換」キーをIMEオン（日本語入力オン）に設定しておく
// iOS機などUS配列でしか認識できない機器で使う場合はデフォルトレイヤーをUSに切り替える

#include "planck.h"
#include "action_layer.h"
#include "keymap_jp.h"    // qmk_firmware/quantum/keymap_extras/keymap_jp.h

extern keymap_config_t keymap_config;

// レイヤー
enum planck_layers {
  _JIS,             // デフォルトレイヤー（JIS配列で認識） MacとWindowsでの共用を想定
  _US,              // デフォルトレイヤー（ US配列で認識） JISで認識させられないiOS機器を想定
  _LOWER,           // Lower
  _RAISE,           // Raise
  _RAISE_US,        // Raise (US配列用)
  _FUNC1,           // Function_1      移動、レイヤートグルなど
  _FUNC2,           // Function_2      マクロ用
  _ADJUST           // Adjust          設定変更など
};

// 特殊な動作のキーコード
enum planck_keycodes {
  JIS = SAFE_RANGE, // デフォルトレイヤーをJIS配列用に切替
  US,               // デフォルトレイヤーをUS配列用に切替
  SFT_JQT,          // タップでJISの「'」  ホールドで右Shift
  WN_SCLN,          // タップでJISの「;」  シフトでJISの「:」
  TGL_RIS,          // トグルでRaiseレイヤーに切り替え
  TGL_LOW,          // トグルでLowerレイヤーに切り替え
  MACRO_1,          // 以下、マクロ
  MACRO_2,
  MACRO_3,
};

// 特殊な動作のマクロ設定
enum user_macro {
  UM_EMHL,
  UM_KHKR,
  UM_KHRU
};
#define M_EMHL MACROTAP(UM_EMHL)      // タップでMacの「英数」とWindowsの「無変換」  ホールドでLowerレイヤー
#define M_KHKR MACROTAP(UM_KHKR)      // タップでMacの「かな」とWindowsの「変換」    ホールドでRaiseレイヤー
#define M_KHRU MACROTAP(UM_KHRU)      // タップでMacの「かな」とWindowsの「変換」    ホールドでRaise(US配列)レイヤー

// ユーザー定義のキーコード
#define FN1_ESC LT(_FUNC1,KC_ESC)     // タップでESC                 ホールドでFunction_1レイヤーon
#define FN2_TAB LT(_FUNC2,KC_TAB)     // タップでTab                 ホールドでFunction_2レイヤーon
#define RSFT_QT MT(MOD_RSFT,KC_QUOT)  // タップで「'」               ホールドで右Shift
#define ADJUST  MO(_ADJUST)           // ホールドでAdjustレイヤーをon
#define RSFT_QT MT(MOD_RSFT,KC_QUOT)  // タップで「'」      ホールドで右Shift       (US配列用)
#define MY_VOLD LALT(LSFT(KC_VOLD))   // 細かいボリュームダウン                     (Mac)
#define MY_VOLU LALT(LSFT(KC_VOLU))   // 細かいボリュームアップ                     (Mac)
#define ALT_PSC LALT(KC_PSCR)         // Alt + PrintScreen                          (Windows)
#define WN_CAPS S(KC_CAPS)            // Caps Lock                                  (Windows)
#define TASK    LCA(KC_DEL)           // タスクマネージャー起動ショートカット       (Windows)
#define TAB_NX  LGUI(KC_TAB)          // 次のタブを選択                             (mac)
#define TAB_PRV LGUI(LSFT(KC_TAB))    // 前のタブを選択                             (mac)
#ifdef SWAP_LCTR_LGUI                 // MacでControlキーとCommandキーを入れ替えている場合の設定
  #define CTL_ZH  GUI_T(KC_ZKHK)      // タップで半角/全角(Windows)  ホールドで左Control(USレイヤー用)
  #define MY_GUI  KC_LCTL
  #define GUI_F1  LCTL(KC_F1)         // ウインドウ切り替えショートカット       (Mac)
#else
  #define CTL_ZH  CTL_T(KC_ZKHK)
  #define MY_GUI  KC_LGUI
  #define GUI_F1  LGUI(KC_F1)
#endif

// Tap Danceの設定
#ifdef TAP_DANCE_ENABLE
  enum {
   X_TAP_DANCE_1 = 0,
  };
  #define TAP_L TD(X_TAP_DANCE_1)     // タップかホールドで左Shift  ダブルタップでLowerレイヤーのトグル
#else
  #define TAP_L KC_LSFT               // Tap Danceが有効でなければ普通の左シフトキーに設定
#endif

// キーマップ
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* デフォルトレイヤー（JIS配列で認識）
 * ,-----------------------------------------------------------------------------------.
 * |Tab/f2|   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |Esc/f1|   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |Enter |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |Shift |   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |'/SFT |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |ZH/Ctl|Adjust| Alt  | GUI  |英数/L|    Space    |かな/R| Left | Down |  Up  |Right |
 * `-----------------------------------------------------------------------------------'
 */
[_JIS] = {
  {FN2_TAB, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
  {FN1_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    WN_SCLN, KC_ENT },
  {TAP_L  , KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_JQT},
  {CTL_ZH,  ADJUST,  KC_LALT, MY_GUI,  M_EMHL,  KC_SPC,  KC_SPC,  M_KHKR,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* デフォルトレイヤー（ US配列で認識） */
[_US] = {
  {FN2_TAB, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},
  {FN1_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT },
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_QT},
  {CTL_ZH,  ADJUST,  KC_LALT, KC_LGUI, M_EMHL,  KC_SPC,  KC_SPC,  M_KHRU,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT}
},

/* Lower
 * ,-----------------------------------------------------------------------------------.
 * |      |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |Esc/FN|      | Vol- | Vol+ | Mute |   .  |   -  |   4  |   5  |   6  |   *  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      | Prev | Next | Play |   ,  |   +  |   1  |   2  |   3  |   /  |   =  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |   0  |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = {
  {_______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC},
  {FN1_ESC, XXXXXXX, MY_VOLD, MY_VOLU, KC_MUTE, KC_DOT,  KC_PMNS, KC_4,    KC_5,    KC_6,    KC_PAST, _______},
  {_______, XXXXXXX, KC_MRWD, KC_MFFD, KC_MPLY, KC_COMM, KC_PPLS, KC_1,    KC_2,    KC_3,    KC_PSLS, KC_PEQL},
  {_______, _______, _______, _______, _______, _______, _______, KC_0,    _______, _______, _______, _______}
},

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |      |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  | Bksp |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      | Vol- | Vol+ | Mute |   \  |   `  |   -  |   =  |   [  |   ]  |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |CpsLck|      | Prev | Next | Play |   |  |   ~  |   _  |   +  |   {  |   }  |CpsLck|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = {
  {_______, KC_EXLM, JP_AT,   KC_HASH, KC_DLR,  KC_PERC, JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, KC_BSPC},
  {_______, XXXXXXX, MY_VOLD, MY_VOLU, KC_MUTE, JP_YEN,  JP_GRV,  KC_MINS, JP_EQL,  JP_LBRC, JP_RBRC, _______},
  {WN_CAPS, XXXXXXX, KC_MRWD, KC_MFFD, KC_MPLY, JP_PIPE, JP_TILD, JP_UNDS, JP_PLUS, JP_LCBR, JP_RCBR, WN_CAPS},
  {_______, _______, _______, _______, XXXXXXX, _______, _______, _______, _______, _______, _______, _______}
},

/* Raise (US配列) */
[_RAISE_US] = {
  {_______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC},
  {_______, XXXXXXX, MY_VOLD, MY_VOLU, KC_MUTE, KC_BSLS, KC_GRV,  KC_MINS, KC_EQL,  KC_LBRC ,KC_RBRC, _______},
  {WN_CAPS, XXXXXXX, KC_MRWD, KC_MFFD, KC_MPLY, KC_PIPE, KC_TILD, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, WN_CAPS},
  {_______, _______, _______, _______, XXXXXXX, _______, _______, _______, _______, _______, _______, _______}
},

/* Function_1  移動、ウインドウやタブの切り替え、レイヤーのトグル
 * ,-----------------------------------------------------------------------------------.
 * |PrintS|      |      |      |      |      | End  |PG Dn |PG Up | Home |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |      |      | ⌘F1 | Left | Down |  Up  |Right |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |^⇧TAB| ^TAB |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |TglLow|             |TglRis| End  |Pg Dn |Pg Up | Home |
 * `-----------------------------------------------------------------------------------'
 */
[_FUNC1] = {
  {ALT_PSC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_END,  KC_PGDN, KC_PGUP, KC_HOME, XXXXXXX, KC_DEL },
  {_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, GUI_F1,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, _______},
  {_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TAB_PRV, TAB_NX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
  {_______, _______, _______, _______, TGL_LOW, _______, _______, TGL_RIS, KC_END,  KC_PGDN, KC_PGUP, KC_HOME}
},

/* Function_2  マクロ
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |Macro1|Macro2|Macro3|      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_FUNC2] = {
  {_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MACRO_1, MACRO_2, MACRO_3, XXXXXXX, XXXXXXX, _______},
  {XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______},
  {XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},

/* Adjust      設定変更など
 * ,-----------------------------------------------------------------------------------.
 * |TaskMN|Reset |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |Power |      |      |Aud on|Audoff|      |      | JIS  |  US  |      |      |      |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|Musoff|      |      |BLtogg| BL - | BL + |BLstep|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = {
  {TASK,    RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______},
  {KC_POWER,XXXXXXX, MU_MOD,  AU_ON,   AU_OFF,  XXXXXXX, XXXXXXX, JIS,     US,      XXXXXXX, XXXXXXX, _______},
  {XXXXXXX, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  XXXXXXX, XXXXXXX, BL_TOGG, BL_DEC,  BL_INC,  BL_STEP, XXXXXXX},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},

};

// サウンド設定
#ifdef AUDIO_ENABLE
  float layer_lock_on_song[][2]  = SONG(LAYER_LOCK_ON_SOUND);   // Layerロック
  float layer_lock_off_song[][2] = SONG(LAYER_LOCK_OFF_SOUND);  // Layerロック解除
  float caps_lock_song[][2]      = SONG(CAPS_LOCK_ON_SOUND);    // Caps Lockキー
  float adjust_on_song[][2]      = SONG(ADJUST_ON_SOUND);       // Adjustキーon
  float adjust_off_song[][2]     = SONG(ADJUST_OFF_SOUND);      // Adjustキーoff
  float push_song[][2]           = SONG(PUSH_SOUND);            // 汎用
#endif

// LED点灯 & Breathing
#ifdef BACKLIGHT_ENABLE
void led_breathing_on(uint8_t breathingspeed, bool breathing) {
    backlight_enable();
    #ifdef BACKLIGHT_BREATHING
      breathing_period_set(breathingspeed);
      if (breathing) {
        breathing_enable();
      } else {
        breathing_pulse();
      }
    #endif
}
// LED消す
void led_breathing_off(void) {
    #ifdef BACKLIGHT_BREATHING
      breathing_period_set(1);
      breathing_self_disable();
    #endif
    backlight_disable();
}
#endif

// レイヤー初期化
void init_layer(void) {
    layer_off(_LOWER);
    layer_off(_RAISE);
    #ifdef BACKLIGHT_ENABLE
      led_breathing_off();
    #endif
}

// 特殊キーが押された時の動作
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    static uint16_t custom_timer;
    static uint8_t prev_shift;
    static bool lshift = false;
    static bool rshift = false;
    static uint8_t dl;
    static uint8_t l_r_layer;

  switch (keycode) {
    case JIS:                               // デフォルトレイヤーをJISに切り替え
      if (record->event.pressed) {
        init_layer();
        set_single_persistent_default_layer(_JIS);
      }
      return false;
      break;
    case US:                                // デフォルトレイヤーをUSに切り替え
      if (record->event.pressed) {
        init_layer();
        set_single_persistent_default_layer(_US);
      }
      return false;
      break;
    case TGL_LOW:                           // Lower/Raiseにトグル
    case TGL_RIS:
      if (record->event.pressed) {
        dl = biton32(default_layer_state);
        if (keycode == TGL_LOW){
            l_r_layer = _LOWER;
        } else if (keycode == TGL_RIS){
            if(dl == _JIS){
               l_r_layer = _RAISE;
            } else if (dl == _US) {
               l_r_layer = _RAISE_US;
            }
        }
        if IS_LAYER_ON(l_r_layer){
            layer_off(l_r_layer);
            #ifdef BACKLIGHT_ENABLE
              led_breathing_off();
            #endif
            #ifdef AUDIO_ENABLE
              PLAY_SONG(layer_lock_off_song);
            #endif
        } else {
            layer_on(l_r_layer);
            #ifdef BACKLIGHT_ENABLE
            if (keycode == TGL_LOW){
                led_breathing_on(3, true);
            } else if (keycode == TGL_RIS){
                led_breathing_on(1, false);
            }
            #endif
            #ifdef AUDIO_ENABLE
              PLAY_SONG(layer_lock_on_song);
            #endif
        }
      }
      return false;
      break;
    case M_EMHL:                           // Lower/RaiseキーのサウンドとLED
    case M_KHKR:
    case M_KHRU:
        if (record->event.pressed) {       // レイヤーがトグルされていれば、そのレイヤーをオフにしてサウンドを鳴らす
          if (IS_LAYER_ON(_LOWER) || IS_LAYER_ON(_RAISE) || IS_LAYER_ON(_RAISE_US)){
                if (keycode == M_EMHL){
                    l_r_layer = _LOWER;
                } else if (keycode == M_KHKR){
                    l_r_layer = _RAISE;
                } else if (keycode == M_KHRU){
                    l_r_layer = _RAISE_US;
                }
            layer_off(l_r_layer);
            #ifdef AUDIO_ENABLE
              PLAY_SONG(layer_lock_off_song);
            #endif
            return false;
            break;
          } else {                         // レイヤーがトグルされていなければキーが押されている間はLED点灯
            #ifdef BACKLIGHT_ENABLE
              if (keycode == M_EMHL){
                  led_breathing_on(3, true);
              } else if ((keycode == M_KHKR) || (keycode == M_KHRU)){
                  led_breathing_on(1, false);
              }
            #endif
          }
        } else {                           // キーを離したらLED消灯
          #ifdef BACKLIGHT_ENABLE
            led_breathing_off();
          #endif
        }
      return true;
      break;
    #ifdef BACKLIGHT_ENABLE
    case FN2_TAB:                          // LED点灯／消灯
       if (record->event.pressed) {
        if (!(IS_LAYER_ON(_LOWER) || IS_LAYER_ON(_RAISE) || IS_LAYER_ON(_RAISE_US))){
          led_breathing_on(2, true);
        }
       } else {
        if (!(IS_LAYER_ON(_LOWER) || IS_LAYER_ON(_RAISE) || IS_LAYER_ON(_RAISE_US))){
          led_breathing_off();
        }
       }
      return true;
      break;
    case FN1_ESC:                          // LED点灯／消灯
       if (record->event.pressed) {
        if (!(IS_LAYER_ON(_LOWER) || IS_LAYER_ON(_RAISE) || IS_LAYER_ON(_RAISE_US))){
          led_breathing_on(4, true);
        }
       } else {
        if (!(IS_LAYER_ON(_LOWER) || IS_LAYER_ON(_RAISE) || IS_LAYER_ON(_RAISE_US))){
          led_breathing_off();
        }
       }
      return true;
      break;
    #endif
    case SFT_JQT:                          // 長押しでシフトキー、単押しで「'」
      if (record->event.pressed) {
        custom_timer = timer_read();
        prev_shift = keyboard_report->mods & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
        register_code (KC_RSFT);
      } else {
            if (timer_elapsed (custom_timer) < TAPPING_TERM) {
                // 長押しでない場合、別のシフトキーが同時に押されていなければ「'」、押されていれば「"」を出力
                if (prev_shift) {
                    register_code(KC_2);
                    unregister_code(KC_2);
                } else {
                    register_code(KC_7);
                    unregister_code(KC_7);
                }
            }
            unregister_code (KC_RSFT);
      }
      return false;
      break;
    case WN_SCLN:                          // 「;」と「:」
      if (record->event.pressed) {
        lshift = keyboard_report->mods & MOD_BIT(KC_LSFT);
        rshift = keyboard_report->mods & MOD_BIT(KC_RSFT);
        if (lshift || rshift) {
          if (lshift) unregister_code(KC_LSFT);
          if (rshift) unregister_code(KC_RSFT);
          register_code(JP_COLN);
          unregister_code(JP_COLN);
          if (lshift) register_code(KC_LSFT);
          if (rshift) register_code(KC_RSFT);
        } else {
          register_code(JP_SCLN);
          unregister_code(JP_SCLN);
        }
      }
      return false;
      break;
    case WN_CAPS:                          // Caps Lockの音を鳴らす
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(caps_lock_song);
        #endif
      }
      return true;
      break;
    case BL_TOGG:                          // LED光量が変更された時に音を鳴らす
    case BL_DEC:
    case BL_INC:
    case BL_STEP:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(push_song);
        #endif
      }
      return true;
      break;
    case ADJUST:                           // Adjustキー用の音
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(adjust_on_song);
        #endif
      } else {
          #ifdef AUDIO_ENABLE
            PLAY_SONG(adjust_off_song);
          #endif
      }
      return true;
      break;
    // 以下、マクロ
    case MACRO_1:
      if (record->event.pressed) {
        SEND_STRING("This is a test.");
      }
      return false;
      break;
    case MACRO_2:
      if (record->event.pressed) {
        SEND_STRING("korehatesutodesu.");
      }
      return false;
      break;
    case MACRO_3:
      if (record->event.pressed) {
        SEND_STRING("sukinamojiwoiretekudasai.");
      }
      return false;
      break;
  }
  return true;
}

// Macro actions for each corresponding ID.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
        case UM_EMHL: // タップで「英数」と「無変換」、ホールドで「Lower」
        return MACRO_TAP_HOLD_LAYER( record, MACRO(T(MHEN), T(LANG2), END), _LOWER );
        case UM_KHKR: // タップで「かな」と「変換」、ホールドで「Raise」
        return MACRO_TAP_HOLD_LAYER( record, MACRO(T(HENK), T(LANG1), END), _RAISE );
        case UM_KHRU: // タップで「かな」と「変換」、ホールドで「Raise(US配列用)」
        return MACRO_TAP_HOLD_LAYER( record, MACRO(T(HENK), T(LANG1), END), _RAISE_US );
        };
        return MACRO_NONE;
}

// Tap danceの設定
#ifdef TAP_DANCE_ENABLE
enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
};

typedef struct {
  bool is_press_action;
  int state;
} tap;

int cur_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || state->pressed==0) return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    return DOUBLE_TAP;
  }
  else return 6; //magic number. At some point this method will expand to work for more presses
}

//instanalize an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
  .is_press_action = true,
  .state = 0
};

void x_finished_1 (qk_tap_dance_state_t *state, void *user_data) {
  xtap_state.state = cur_dance(state);
  switch (xtap_state.state) {
    case SINGLE_TAP: register_code(KC_LSFT); break;
    case SINGLE_HOLD: register_code(KC_LSFT); break;
    case DOUBLE_TAP:
        layer_invert(_LOWER);
        if (IS_LAYER_ON(_LOWER)){
            #ifdef BACKLIGHT_ENABLE
              led_breathing_on(3, true);
            #endif
            #ifdef AUDIO_ENABLE
              PLAY_SONG(layer_lock_on_song);
            #endif
        } else {
        #ifdef BACKLIGHT_ENABLE
          led_breathing_off();
        #endif
        #ifdef AUDIO_ENABLE
            PLAY_SONG(layer_lock_off_song);
        #endif
        }
         break;
  }
}

void x_reset_1 (qk_tap_dance_state_t *state, void *user_data) {
  switch (xtap_state.state) {
    case SINGLE_TAP: unregister_code(KC_LSFT); break;
    case SINGLE_HOLD: unregister_code(KC_LSFT); break;
    case DOUBLE_TAP: break;
  }
  xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
 [X_TAP_DANCE_1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished_1, x_reset_1),
};
#endif
