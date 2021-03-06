#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "connected_devices/connected_devices.hpp"
#include <iostream>

TEST_CASE("connected_devices::details::descriptions") {
  {
    krbn::connected_devices::details::descriptions descriptions1("manufacturer1",
                                                                 "product1");
    krbn::connected_devices::details::descriptions descriptions2("manufacturer2",
                                                                 "product2");
    krbn::connected_devices::details::descriptions descriptions3("manufacturer1",
                                                                 "product1");

    REQUIRE(descriptions1.get_manufacturer() == "manufacturer1");
    REQUIRE(descriptions1.get_product() == "product1");

    REQUIRE(descriptions1.to_json() == nlohmann::json({
                                           {"manufacturer", "manufacturer1"},
                                           {"product", "product1"},
                                       }));

    REQUIRE(descriptions1 == descriptions3);
    REQUIRE(descriptions1 != descriptions2);
  }
  {
    krbn::connected_devices::details::descriptions descriptions1(nlohmann::json(nullptr));
    krbn::connected_devices::details::descriptions descriptions2(nlohmann::json({
        {"manufacturer", "manufacturer2"},
        {"product", "product2"},
    }));

    REQUIRE(descriptions1.get_manufacturer() == "");
    REQUIRE(descriptions1.get_product() == "");

    REQUIRE(descriptions2.get_manufacturer() == "manufacturer2");
    REQUIRE(descriptions2.get_product() == "product2");
  }
}

TEST_CASE("connected_devices::details::device") {
  {
    krbn::connected_devices::details::descriptions descriptions("manufacturer1",
                                                                "product1");
    krbn::device_identifiers identifiers(krbn::vendor_id(1234),
                                         krbn::product_id(5678),
                                         true,
                                         false);
    krbn::connected_devices::details::device device(descriptions,
                                                    identifiers,
                                                    true,
                                                    true);

    REQUIRE(device.get_descriptions() == descriptions);
    REQUIRE(device.get_identifiers() == identifiers);
    REQUIRE(device.get_is_built_in_keyboard() == true);
    REQUIRE(device.get_is_built_in_trackpad() == true);

    REQUIRE(device.to_json() == nlohmann::json(
                                    {{
                                         "descriptions",
                                         {
                                             {
                                                 "manufacturer",
                                                 "manufacturer1",
                                             },
                                             {
                                                 "product",
                                                 "product1",
                                             },
                                         },
                                     },
                                     {
                                         "identifiers",
                                         {
                                             {
                                                 "vendor_id",
                                                 1234,
                                             },
                                             {
                                                 "product_id",
                                                 5678,
                                             },
                                             {
                                                 "is_keyboard",
                                                 true,
                                             },
                                             {
                                                 "is_pointing_device",
                                                 false,
                                             },
                                         },
                                     },
                                     {
                                         "is_built_in_keyboard",
                                         true,
                                     },
                                     {
                                         "is_built_in_trackpad",
                                         true,
                                     }}));
  }
  {
    krbn::connected_devices::details::device device1(nlohmann::json(nullptr));
    krbn::connected_devices::details::device device2(nlohmann::json(
        {{
             "descriptions",
             {
                 {
                     "manufacturer",
                     "manufacturer2",
                 },
                 {
                     "product",
                     "product2",
                 },
             },
         },
         {
             "identifiers",
             {
                 {
                     "vendor_id",
                     1234,
                 },
                 {
                     "product_id",
                     5678,
                 },
                 {
                     "is_keyboard",
                     true,
                 },
                 {
                     "is_pointing_device",
                     false,
                 },
             },
         },
         {
             "is_built_in_keyboard",
             true,
         },
         {
             "is_built_in_trackpad",
             true,
         }}));

    REQUIRE(device1.get_descriptions().get_manufacturer() == "");
    REQUIRE(device1.get_descriptions().get_product() == "");
    REQUIRE(device1.get_identifiers().get_vendor_id() == krbn::vendor_id(0));
    REQUIRE(device1.get_identifiers().get_product_id() == krbn::product_id(0));
    REQUIRE(device1.get_identifiers().get_is_keyboard() == false);
    REQUIRE(device1.get_identifiers().get_is_pointing_device() == false);
    REQUIRE(device1.get_is_built_in_keyboard() == false);
    REQUIRE(device1.get_is_built_in_trackpad() == false);

    REQUIRE(device2.get_descriptions().get_manufacturer() == "manufacturer2");
    REQUIRE(device2.get_descriptions().get_product() == "product2");
    REQUIRE(device2.get_identifiers().get_vendor_id() == krbn::vendor_id(1234));
    REQUIRE(device2.get_identifiers().get_product_id() == krbn::product_id(5678));
    REQUIRE(device2.get_identifiers().get_is_keyboard() == true);
    REQUIRE(device2.get_identifiers().get_is_pointing_device() == false);
    REQUIRE(device2.get_is_built_in_keyboard() == true);
    REQUIRE(device2.get_is_built_in_trackpad() == true);
  }
}

TEST_CASE("connected_devices") {
  {
    krbn::connected_devices::connected_devices connected_devices;
    REQUIRE(connected_devices.to_json() == nlohmann::json::array());
  }
  {
    krbn::connected_devices::connected_devices connected_devices;

    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer1",
                                                                  "product1");
      krbn::device_identifiers identifiers(krbn::vendor_id(1234),
                                           krbn::product_id(5678),
                                           true,
                                           false);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      true,
                                                      false);
      connected_devices.push_back_device(device);
    }
    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer1 (ignored)",
                                                                  "product1 (ignored)");
      krbn::device_identifiers identifiers(krbn::vendor_id(1234),
                                           krbn::product_id(5678),
                                           true,
                                           false);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      true,
                                                      false);
      connected_devices.push_back_device(device);
    }
    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer3",
                                                                  "product3");
      krbn::device_identifiers identifiers(krbn::vendor_id(2345),
                                           krbn::product_id(6789),
                                           false,
                                           true);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      false,
                                                      false);
      connected_devices.push_back_device(device);
    }
    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer2",
                                                                  "product2");
      krbn::device_identifiers identifiers(krbn::vendor_id(1234),
                                           krbn::product_id(5679),
                                           false,
                                           true);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      false,
                                                      true);
      connected_devices.push_back_device(device);
    }
    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer1",
                                                                  "product4");
      krbn::device_identifiers identifiers(krbn::vendor_id(123),
                                           krbn::product_id(678),
                                           false,
                                           true);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      false,
                                                      true);
      connected_devices.push_back_device(device);
    }
    {
      krbn::connected_devices::details::descriptions descriptions("manufacturer1",
                                                                  "product4");
      krbn::device_identifiers identifiers(krbn::vendor_id(123),
                                           krbn::product_id(678),
                                           true,
                                           false);
      krbn::connected_devices::details::device device(descriptions,
                                                      identifiers,
                                                      true,
                                                      false);
      connected_devices.push_back_device(device);
    }

    REQUIRE(connected_devices.is_loaded() == false);
    REQUIRE(connected_devices.get_devices().size() == 5);
    REQUIRE(connected_devices.get_devices()[0].get_identifiers().get_vendor_id() == krbn::vendor_id(1234));
    REQUIRE(connected_devices.get_devices()[0].get_identifiers().get_product_id() == krbn::product_id(5678));
    REQUIRE(connected_devices.get_devices()[1].get_identifiers().get_vendor_id() == krbn::vendor_id(1234));
    REQUIRE(connected_devices.get_devices()[1].get_identifiers().get_product_id() == krbn::product_id(5679));
    REQUIRE(connected_devices.get_devices()[2].get_identifiers().get_vendor_id() == krbn::vendor_id(2345));
    REQUIRE(connected_devices.get_devices()[2].get_identifiers().get_product_id() == krbn::product_id(6789));

    std::ifstream ifs("json/connected_devices.json");

    REQUIRE(connected_devices.to_json() == nlohmann::json::parse(ifs));
  }

  {
    krbn::connected_devices::connected_devices connected_devices("json/connected_devices.json");

    REQUIRE(connected_devices.is_loaded() == true);
    REQUIRE(connected_devices.get_devices().size() == 5);
    REQUIRE(connected_devices.get_devices()[0].get_is_built_in_keyboard() == true);
    REQUIRE(connected_devices.get_devices()[0].get_is_built_in_trackpad() == false);
    REQUIRE(connected_devices.get_devices()[1].get_is_built_in_keyboard() == false);
    REQUIRE(connected_devices.get_devices()[1].get_is_built_in_trackpad() == true);
  }

  {
    krbn::connected_devices::connected_devices connected_devices("json/not_found.json");

    REQUIRE(connected_devices.is_loaded() == false);
    REQUIRE(connected_devices.get_devices().size() == 0);
  }

  {
    krbn::connected_devices::connected_devices connected_devices("json/broken.json");

    REQUIRE(connected_devices.is_loaded() == false);
    REQUIRE(connected_devices.get_devices().size() == 0);
  }
}
