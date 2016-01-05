#include "SettingsPageComponent.h"
#include "Main.h"
#include "Utils.h"

SettingsCategoryItemComponent::SettingsCategoryItemComponent(const Drawable *iconImage)
: icon{ new DrawableButton("icon", DrawableButton::ImageFitted) },
  toggle{ new SwitchComponent() },
  button{ new TextButton() } {
  icon->setImages(iconImage);
  toggle->addListener(this);
  addAndMakeVisible(icon);
  addAndMakeVisible(toggle);
  addChildComponent(button);
}

void SettingsCategoryItemComponent::paint(Graphics &g) {}

void SettingsCategoryItemComponent::resized() {
  auto b = getLocalBounds();
  auto h = b.getHeight();

  int spacing = 10;
  int togWidth = h * 1.1f;

  layout.setItemLayout(0, h, h, h);
  layout.setItemLayout(1, spacing, spacing, spacing);
  layout.setItemLayout(2, togWidth, togWidth, togWidth);
  layout.setItemLayout(3, spacing, spacing, spacing);
  layout.setItemLayout(4, h, -1, -1);

  Component *comps[] = { icon, nullptr, toggle, nullptr, button };
  layout.layOutComponents(comps, 5, b.getX(), b.getY(), b.getWidth(), b.getHeight(), false, true);
}

void SettingsCategoryItemComponent::buttonClicked(Button *b) {}

void SettingsCategoryItemComponent::buttonStateChanged(Button *b) {
  if (b == toggle) {
    button->setVisible(toggle->getToggleState());
  }
}

SettingsPageComponent::SettingsPageComponent() {
  mainPage = new Component();
  addAndMakeVisible(mainPage);
  mainPage->toBack();

  ScopedPointer<Drawable> brightLo = Drawable::createFromImageData(
      BinaryData::brightnessIconLo_png, BinaryData::brightnessIconLo_pngSize);
  ScopedPointer<Drawable> brightHi = Drawable::createFromImageData(
      BinaryData::brightnessIconHi_png, BinaryData::brightnessIconHi_pngSize);
  screenBrightnessSlider =
      ScopedPointer<IconSliderComponent>(new IconSliderComponent(*brightLo, *brightHi));

  ScopedPointer<Drawable> volLo =
      Drawable::createFromImageData(BinaryData::volumeIconLo_png, BinaryData::volumeIconLo_pngSize);
  ScopedPointer<Drawable> volHi =
      Drawable::createFromImageData(BinaryData::volumeIconHi_png, BinaryData::volumeIconHi_pngSize);
  volumeSlider = ScopedPointer<IconSliderComponent>(new IconSliderComponent(*volLo, *volHi));

  mainPage->addAndMakeVisible(screenBrightnessSlider);
  mainPage->addAndMakeVisible(volumeSlider);

  // create back button
  ScopedPointer<Drawable> backButtonDrawable =
      Drawable::createFromImageData(BinaryData::backIcon_png, BinaryData::backIcon_pngSize);
  backButton = createImageButtonFromDrawable("Back", *backButtonDrawable);
  backButton->addListener(this);
  backButton->setAlwaysOnTop(true);
  addAndMakeVisible(backButton);

  wifiIcon = Drawable::createFromImageData(BinaryData::wifiIcon_png, BinaryData::wifiIcon_pngSize);
  bluetoothIcon = Drawable::createFromImageData(BinaryData::bluetoothIcon_png,
                                                BinaryData::bluetoothIcon_pngSize);

  wifiCategoryItem = new SettingsCategoryItemComponent(wifiIcon);
  wifiCategoryItem->button->addListener(this);
  addAndMakeVisible(wifiCategoryItem);

  bluetoothCategoryItem = new SettingsCategoryItemComponent(bluetoothIcon);
  bluetoothCategoryItem->button->addListener(this);
  addAndMakeVisible(bluetoothCategoryItem);

  addAndMakeVisible(screenBrightnessSlider);
  addAndMakeVisible(volumeSlider);

  wifiPage = new SettingsPageWifiComponent();
  bluetoothPage = new SettingsPageBluetoothComponent();
}

SettingsPageComponent::~SettingsPageComponent() {}

void SettingsPageComponent::paint(Graphics &g) {}

void SettingsPageComponent::resized() {
  auto bounds = getLocalBounds();

  {
    for (int i = 0, j = 0; i < 4; ++i) {
      if (i > 0) verticalLayout.setItemLayout(j++, 0, -1, -1);
      verticalLayout.setItemLayout(j++, 48, 48, 48);
    }

    Component *settingsItems[] = { wifiCategoryItem,       nullptr, bluetoothCategoryItem, nullptr,
                                   screenBrightnessSlider, nullptr, volumeSlider };
    auto b = bounds.reduced(10);
    b.setLeft(70);
    verticalLayout.layOutComponents(settingsItems, 7, b.getX(), b.getY(), b.getWidth(),
                                    b.getHeight(), true, true);
  }

  mainPage->setBounds(bounds);

  backButton->setBounds(bounds.getX(), bounds.getY(), 60, bounds.getHeight());
}

void SettingsPageComponent::buttonClicked(Button *button) {
  if (button == backButton) {
    getMainStack().popPage(PageStackComponent::kTransitionTranslateHorizontal);
  } else if (button == wifiCategoryItem->button) {
    getMainStack().pushPage(wifiPage, PageStackComponent::kTransitionTranslateHorizontal);
  } else if (button == bluetoothCategoryItem->button) {
    getMainStack().pushPage(bluetoothPage, PageStackComponent::kTransitionTranslateHorizontal);
  }
}
