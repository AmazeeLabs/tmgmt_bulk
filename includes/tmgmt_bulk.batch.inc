<?php

//AXXX
function _tmgmt_bulk_search_and_store($plugin, $item_type, $filters) {
  $translated_to = $filters['translated_to'];
  $not_exists_in_jobs = $filters['not_exists_in_jobs'];
  $specific = isset($filters['specific']) ? $filters['specific'] : array();
  $_SESSION['tmgmt_bulk_storage'][$plugin][$item_type] = _tmgmt_bulk_get_selector($plugin)->searchItems($item_type, $translated_to, $not_exists_in_jobs, $specific);
}

//AXXX
function _tmgmt_bulk_request_translation($items) {
  if (!isset($_SESSION['tmgmt_bulk_jobs_by_source_language'])) {
    $_SESSION['tmgmt_bulk_jobs_by_source_language'] = array();
  }
  $jobs_by_source_language = array();
  $languages = array();
  foreach (language_list() as $langcode => $language) {
    $languages[$langcode] = $language->name;
  }
  $target_language = $_SESSION['tmgmt_bulk_target_language'];
  foreach ($items as $item) {
    $job_item = tmgmt_job_item_create($item['plugin'], $item['item_type'], $item['item_id']);
    $job_item->save();
    $source_language = $job_item->getSourceLangCode();
    if (isset($_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language])) {
      if (isset($jobs_by_source_language[$source_language])) {
        $job = $jobs_by_source_language[$source_language];
      }
      else {
        $job = $jobs_by_source_language[$source_language] = tmgmt_job_load($_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language]);
      }
    }
    else {
      $job = $jobs_by_source_language[$source_language] = tmgmt_job_create($source_language, $target_language);
      $job->save();
      $uri = $job->uri();
      drupal_set_message(t('Created <a href="!url">translation job</a> with %language source language', array(
        '!url' => url($uri['path']),
        '%language' => $languages[$source_language],
      )));
      $_SESSION['tmgmt_bulk_jobs_by_source_language'][$source_language] = $job->tjid;
    }
    $job->addExistingItem($job_item);
  }
}
